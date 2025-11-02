#ifndef RUCT_H
#define RUCT_H

#ifdef RUCT_STRIP_PREFIX
#define RUCT_PREFIX(x) x
#define RUCT_VEC_PREFIX(x) Vec##x
#else
#define RUCT_PREFIX(x) Ruct_##x
#define RUCT_VEC_PREFIX(x) Ruct_Vec##x
#endif

#define RUCT_VEC_INITIAL_CAPACITY 1024

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {} RUCT_PREFIX(None);
#define RUCT_NONE (RUCT_PREFIX(None)){}

#define let auto

typedef u_int8_t u8;
typedef u_int16_t u16;
typedef u_int32_t u32;
typedef u_int64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

typedef size_t usize;

#define loop while(1)

#define RUCT_TRY_DISCARD(result)        \
do {                                    \
    if (!result.is_ok) return result;   \
} while(0)

#define RUCT_TRY(result)                        \
result.ok;                                      \
do {                                            \
    if (!result.is_ok) return result;           \
} while (0)

#define RUCT_TRY_CONVERT(result, target_err)                \
result.ok;                                                  \
do {                                                        \
    if (!result.is_ok) return target_err(result.error);     \
} while (0)

#define RUCT_OK_NONE Ruct_Ok_None(RUCT_NONE)

#define RUCT_MAIN(func) \
int main() {                                    \
Ruct_Result_None res = __RUCT_APPMODULE_##func();       \
    if (!res.is_ok) {                           \
        fprintf(stderr, "ERROR: %s\n", res.error);       \
        return -1;                              \
    }                                           \
    return 0;\
}                                               \

#define RUCT_APPMODULE(func, ...)  \
Ruct_Result_None __RUCT_APPMODULE_##func () {  \
    __VA_ARGS__                         \
    return RUCT_OK_NONE;                \
}

#define RUCT_RUN_APPMODULE(func) __RUCT_APPMODULE_##func()

#define RUCT_TESTMODULE(func, ...)   \
Ruct_Result_None __RUCT_TESTMODULE_##func () {                \
    usize success = 0;    \
    usize failed    = 0;    \
    __VA_ARGS__             \
    printf("%s: %ld/%ld passed\n", #func, success, success + failed);             \
    if (failed > 0) {           \
        return Ruct_Err_None("One or more tests failed.");              \
    }                           \
    return RUCT_OK_NONE;               \
}

#define RUCT_RUN_TESTMODULE(func) __RUCT_TESTMODULE_##func ()

#define RUCT_TEST_DEFINE(func, ...) \
Ruct_Result_None func() {       \
    __VA_ARGS__                 \
    return RUCT_OK_NONE;            \
}

#define RUCT_TEST(func) do {       \
    printf("running test %s... ", #func);          \
    Ruct_Result_None result = func ();                       \
    if (!result.is_ok) {                \
        printf("fail!\n");      \
        printf("Err: %s\n", result.error);        \
        failed++;               \
    } else {                        \
        printf("ok!\n");            \
        success++;                  \
    }                               \
} while (0)

#define RUCT_TEST_FAIL(func) do {\
printf("running test %s... ", #func);          \
Ruct_Result_None result = func ();                       \
if (!result.is_ok) {                \
    printf("ok!\n");            \
    success++;                  \
} else {                        \
    printf("fail!\n");      \
    failed++;               \
}                               \
} while (0)

#define RUCT_ASSERT_EQ(desc, v1, v2) do { \
    if (v1 != v2) {                             \
        return Ruct_Err_None(desc);\
    }                                                         \
} while(0)

#define RUCT_ASSERT_FEQ(desc, v1, v2) do {   \
    if (!(fabs((f64)v1 - (f64)v2) < (f64)1e-6)) {   \
        return Ruct_Err_None(desc);             \
    }                                   \
} while(0)

#define RUCT_PANIC(msg) do {                    \
    fprintf(stderr, "PANIC: %s\n", msg);        \
    abort();                                    \
} while (0)                                     \

#define RUCT_DEFINE_RESULT_TYPE(TypeName, Type)                            \
typedef struct {                                                            \
    bool is_ok;                                                             \
    union {                                                                 \
        Type ok;                                                           \
        const char* error;                                                  \
    };                                                                      \
} RUCT_PREFIX(Result_##TypeName);                                                    \
                                                                            \
static inline RUCT_PREFIX(Result_##TypeName) RUCT_PREFIX(Ok_##TypeName)(Type value) {        \
    return (RUCT_PREFIX(Result_##TypeName)){ .is_ok = 1, .ok = value };              \
}                                                                           \
                                                                            \
static inline RUCT_PREFIX(Result_##TypeName) RUCT_PREFIX(Err_##TypeName)(const char* msg) {   \
    return (RUCT_PREFIX(Result_##TypeName)) { .is_ok = 0, .error = msg };            \
}                                                                           \
                                                                            \
static inline Type RUCT_PREFIX(unwrap_##TypeName)(RUCT_PREFIX(Result_##TypeName) result) {   \
    if (!result.is_ok) { RUCT_PANIC(result.error); }                        \
    else { return result.ok; }                                              \
}

#define RUCT_DEFINE_OPTION_TYPE(TypeName, Type)                             \
typedef struct {                                                            \
    bool is_some;                                                           \
    Type some;                                                              \
} RUCT_PREFIX(Option_##TypeName);                                                    \
                                                                            \
static inline RUCT_PREFIX(Option_##TypeName) RUCT_PREFIX(Some_##TypeName)(Type value) {       \
    return (RUCT_PREFIX(Option_##TypeName)){ .is_some = 1, .some = value };          \
}                                                                           \
                                                                            \
static inline RUCT_PREFIX(Option_##TypeName) RUCT_PREFIX(None_##TypeName)() {                 \
    return (RUCT_PREFIX(Option_##TypeName)){ .is_some = 0 };                         \
}                                                                           \

#define RUCT_DEFINE_VEC_TYPE(TypeName, Type)            \
typedef struct {                                        \
    Type* ptr;                                          \
    usize len;                                          \
    usize capacity;                                     \
} RUCT_PREFIX(Vec##TypeName);                           \

#define RUCT_DEFINE_VEC_TYPE_METHODS(TypeName, Type)                                                        \
static inline RUCT_PREFIX(Result_Vec##TypeName) RUCT_PREFIX(new_Vec##TypeName)() {                            \
    RUCT_PREFIX(Vec##TypeName) new_vec = { .ptr = NULL, .len = 0, .capacity = RUCT_VEC_INITIAL_CAPACITY };   \
    new_vec.ptr = (Type*) malloc(sizeof(Type) * new_vec.capacity);                                            \
    if (new_vec.ptr == NULL) { \
        RUCT_PREFIX(Result_Vec##TypeName) result = RUCT_PREFIX(Err_Vec##TypeName)("malloc failure");      \
        return result;      \
    }   \
    RUCT_PREFIX(Result_Vec##TypeName) result = RUCT_PREFIX(Ok_Vec##TypeName)(new_vec);                  \
    return result;                                                                                          \
    }   \
    static inline void RUCT_PREFIX(push_Vec##TypeName)(RUCT_PREFIX(Vec##TypeName*) vec, Type value) { \
        if(vec->capacity == vec->len + 1) {                     \
            vec->capacity *= 2;                                  \
            vec->ptr = (Type*) realloc(vec->ptr, sizeof(Type) * vec->capacity);           \
        }       \
        *(vec->ptr + vec->len)  = value;                            \
        vec->len += 1;                                                  \
    }               \
    static inline RUCT_PREFIX(Option_##TypeName) RUCT_PREFIX(remove_Vec##TypeName)(RUCT_PREFIX(Vec##TypeName)* vec, usize i) {   \
        if (i >= vec->len) { return RUCT_PREFIX(None_##TypeName()); }                           \
        RUCT_PREFIX(Option_##TypeName) opt = RUCT_PREFIX(Some_##TypeName(*(vec->ptr + i)));     \
        vec->len--;                                                                             \
        if (i == vec->len) { return opt; }                                                  \
        else {                                                                                  \
            for (usize j = i; j < vec->len; j++) {                                              \
                *(vec->ptr + j) = *(vec->ptr + j + 1);                                          \
            }                                                                                   \
        }                                                                                       \
        return opt;                                                                             \
    }                                                                                           \
    static inline void RUCT_PREFIX(free_Vec##TypeName)(RUCT_PREFIX(Vec##TypeName*) vec) {   \
        free(vec->ptr);      \
    }\
    static inline RUCT_PREFIX(Option_##TypeName) RUCT_PREFIX(get_Vec##TypeName)(RUCT_PREFIX(Vec##TypeName*) vec, usize i) { \
        if (i >= vec->len) { return RUCT_PREFIX(None_##TypeName()); } \
        return RUCT_PREFIX(Some_##TypeName)( *(vec->ptr + i));    \
    }   \
    static inline RUCT_PREFIX(Option_p##TypeName) RUCT_PREFIX(getPtr_Vec##TypeName)(RUCT_PREFIX(Vec##TypeName*) vec, usize i) { \
        if (i >= vec->len) { return RUCT_PREFIX(None_p##TypeName()); } \
        return RUCT_PREFIX(Some_p##TypeName)( (vec->ptr + i) );    \
    }   \

// generates Vec, Option and Result types (no methods for Vec type)
#define RUCT_DEFINE_BASE_TYPES(Type)              \
    RUCT_DEFINE_VEC_TYPE( TypeName, Type);         \
    RUCT_DEFINE_OPTION_TYPE( TypeName, Type);      \
    RUCT_DEFINE_RESULT_TYPE( TypeName, Type);      \
    RUCT_DEFINE_OPTION_TYPE( p##TypeName, Type*);  \
    RUCT_DEFINE_RESULT_TYPE( p##TypeName, Type*);

#define RUCT_DEFINE_CUSTOM_TYPE(Type)     \
    RUCT_DEFINE_VEC_TYPE( Type, RUCT_PREFIX(Type));         \
    RUCT_DEFINE_OPTION_TYPE( Type, RUCT_PREFIX(Type));      \
    RUCT_DEFINE_RESULT_TYPE( Type, RUCT_PREFIX(Type));      \
    RUCT_DEFINE_OPTION_TYPE( p##Type, RUCT_PREFIX(Type*));  \
    RUCT_DEFINE_RESULT_TYPE( p##Type, RUCT_PREFIX(Type*));  \
    RUCT_DEFINE_OPTION_TYPE( Vec##Type, RUCT_VEC_PREFIX(Type)); \
    RUCT_DEFINE_RESULT_TYPE( Vec##Type, RUCT_VEC_PREFIX(Type)); \
    RUCT_DEFINE_VEC_TYPE_METHODS( Type, RUCT_PREFIX(Type));

#define RUCT_PRIMITIVE_TYPES    \
    X(I8, i8)                   \
    X(I16, i16)                 \
    X(I32, i32)                 \
    X(I64, i64)                 \
    X(U8, u8)                   \
    X(U16, u16)                 \
    X(U32, u32)                 \
    X(U64, u64)                 \
    X(USize, usize)             \
    X(F32, f32)                 \
    X(F64, f64)                 \
    X(Char, char)               \
    X(Bool, bool)               \
    X(None, RUCT_PREFIX(None))  \

#define RUCT_PRIMITIVE_STAR_TYPES  \
    X(pI8, i8*)                    \
    X(pI16, i16*)                  \
    X(pI32, i32*)                  \
    X(pI64, i64*)                  \
    X(pU8, u8*)                    \
    X(pU16, u16*)                  \
    X(pU32, u32*)                  \
    X(pU64, u64*)                  \
    X(pUSize, usize*)              \
    X(pF32, f32*)                  \
    X(pF64, f64*)                  \
    X(pChar, char*)                \
    X(pBool, bool*)                \
    X(pNone, RUCT_PREFIX(None*))   \

#define RUCT_PRIMITIVE_VEC_TYPES    \
    X(VecI8, RUCT_PREFIX(VecI8))                 \
    X(VecI16, RUCT_PREFIX(VecI16))                 \
    X(VecI32, RUCT_PREFIX(VecI32))                 \
    X(VecI64, RUCT_PREFIX(VecI64))                 \
    X(VecU8, RUCT_PREFIX(VecU8))                   \
    X(VecU16, RUCT_PREFIX(VecU16))                 \
    X(VecU32, RUCT_PREFIX(VecU32))                 \
    X(VecU64, RUCT_PREFIX(VecU64))                 \
    X(VecUSize, RUCT_PREFIX(VecUSize))             \
    X(VecF32, RUCT_PREFIX(VecF32))                 \
    X(VecF64, RUCT_PREFIX(VecF64))                 \
    X(VecChar, RUCT_PREFIX(VecChar))               \
    X(VecBool, RUCT_PREFIX(VecBool))               \
    X(VecNone, RUCT_PREFIX(VecNone))  \

#define RUCT_PRIMITIVE_STAR_VEC_TYPES    \
    X(pVecI8, RUCT_PREFIX(VecpI8))                 \
    X(pVecI16, RUCT_PREFIX(VecpI16))                 \
    X(pVecI32, RUCT_PREFIX(VecpI32))                 \
    X(pVecI64, RUCT_PREFIX(VecpI64))                 \
    X(pVecU8, RUCT_PREFIX(VecpU8))                   \
    X(pVecU16, RUCT_PREFIX(VecpU16))                 \
    X(pVecU32, RUCT_PREFIX(VecpU32))                 \
    X(pVecU64, RUCT_PREFIX(VecpU64))                 \
    X(pVecUSize, RUCT_PREFIX(VecpUSize))             \
    X(pVecF32, RUCT_PREFIX(VecpF32))                 \
    X(pVecF64, RUCT_PREFIX(VecpF64))                 \
    X(pVecChar, RUCT_PREFIX(VecpChar))               \
    X(pVecBool, RUCT_PREFIX(VecpBool))               \
    X(pVecNone, RUCT_PREFIX(VecpNone))  \


#   define X(name, type) RUCT_DEFINE_VEC_TYPE(name, type)
RUCT_PRIMITIVE_TYPES
RUCT_PRIMITIVE_STAR_TYPES
#   undef X
#   define X(name, type) RUCT_DEFINE_OPTION_TYPE(name, type)
RUCT_PRIMITIVE_TYPES
RUCT_PRIMITIVE_STAR_TYPES
RUCT_PRIMITIVE_VEC_TYPES
RUCT_PRIMITIVE_STAR_VEC_TYPES
#   undef X
#   define X(name, type) RUCT_DEFINE_RESULT_TYPE(name, type)
RUCT_PRIMITIVE_TYPES
RUCT_PRIMITIVE_STAR_TYPES
RUCT_PRIMITIVE_VEC_TYPES
RUCT_PRIMITIVE_STAR_VEC_TYPES
#   undef X
#   define X(name, type) RUCT_DEFINE_VEC_TYPE_METHODS(name, type)
RUCT_PRIMITIVE_TYPES
#   undef X
#undef RUCT_PRIMITIVE_TYPES
#undef RUCT_PRIMITIVE_STAR_TYPES
#undef RUCT_PRIMITIVE_VEC_TYPES
#undef RUCT_PRIMITIVE_STAR_VEC_TYPES

#ifdef RUCT_STRIP_PREFIX
#   define PANIC(msg) RUCT_PANIC(msg)
#   define DEFINE_RESULT_TYPE(TypeName, Type) RUCT_DEFINE_RESULT_TYPE(TypeName, Type)
#   define DEFINE_OPTION_TYPE(TypeName, Type) RUCT_DEFINE_OPTION_TYPE(TypeName, Type)
#endif

#endif
