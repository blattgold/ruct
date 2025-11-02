#include "ruct.h"

Ruct_Result_None test_try_fails_helper() {
    return Ruct_Err_None("from the helper");
}
RUCT_TEST_DEFINE(test_try_fails,
    Ruct_None n = RUCT_TRY(test_try_fails_helper());
)

Ruct_Result_None test_try_succeeds_helper() {
    return RUCT_OK_NONE;
}
RUCT_TEST_DEFINE(test_try_succeeds,
    Ruct_None n = RUCT_TRY(test_try_succeeds_helper());
)

Ruct_Result_U32 test_try_convert_fails_helper() {
    return Ruct_Err_U32("failed successfully.");
}
RUCT_TEST_DEFINE(test_try_convert_fails,
    u32 v = RUCT_TRY_CONVERT(test_try_convert_fails_helper(), Ruct_Err_None);
)

RUCT_TESTMODULE( ruct_tests,
    RUCT_TEST_FAIL(test_try_fails);
    RUCT_TEST(test_try_succeeds);
    RUCT_TEST_FAIL(test_try_convert_fails);
)

RUCT_APPMODULE(appmod_test,
    return Ruct_Err_None("from app module");
)
RUCT_TEST_DEFINE(test_appmod_err,
    Ruct_None n = RUCT_TRY(RUCT_RUN_APPMODULE(appmod_test));
)

RUCT_TESTMODULE(ruct_appmod_tests,
    RUCT_TEST_FAIL(test_appmod_err);
)

int main() {
    RUCT_RUN_TESTMODULE(ruct_tests);
    RUCT_RUN_TESTMODULE(ruct_appmod_tests);
}
