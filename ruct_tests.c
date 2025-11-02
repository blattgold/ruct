#include "ruct.h"

Ruct_Result_None test_try_fails_helper() {
    return Ruct_Err_None("from the helper");
}
RUCT_TEST_DEFINE(test_try_fails,
    Ruct_None n = RUCT_TRY(test_try_fails_helper());
    return RUCT_OK_NONE;
)

Ruct_Result_None test_try_succeeds_helper() {
    return RUCT_OK_NONE;
};
RUCT_TEST_DEFINE(test_try_succeeds,
    Ruct_None n = RUCT_TRY(test_try_succeeds_helper());
)

RUCT_TEST_MODULE( "ruct_tests",
    RUCT_TEST_FAIL(test_try_fails);
    RUCT_TEST(test_try_succeeds);
)
