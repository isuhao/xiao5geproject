#ifndef _XIAO5GEPROJECT_ZTEST_H_
#define _XIAO5GEPROJECT_ZTEST_H_

/**
 * @file     ztest.h
 * @brief    A lightweight framework for c++ program's unit testing
 * @author   Wu Zesheng <wuzesheng@wuzesheng.com>
 * @since    2010-09-04
 * @version  1.0.0
 */

#include <stdexcept>

//expect a condition to be true
#define Z_EXPECT_TRUE(cond)                                  \
    do                                                       \
    {                                                        \
        if (!(cond))                                         \
        {                                                    \
            throw std::runtime_error("[Check failed] "#cond);\
        }                                                    \
    } while (0);
//expect a condition to be false
#define Z_EXPECT_FALSE(cond)   Z_EXPECT_TRUE(!(cond))

//expect a arithmetic expression's value to be true
#define Z_EXPECT_OP(op, val1, val2)                                          \
    do                                                                       \
    {                                                                        \
        if (!((val1) op (val2)))                                             \
        {                                                                    \
            throw std::runtime_error("[Check failed] "#val1" "#op" "#val2);  \
        }                                                                    \
    } while (0);
//special cases of arithmetic expression: ==, !=, >=, <=, > and <
#define Z_EXPECT_EQ(val1, val2) Z_EXPECT_OP(==, val1, val2)
#define Z_EXPECT_NE(val1, val2) Z_EXPECT_OP(!=, val1, val2)
#define Z_EXPECT_GE(val1, val2) Z_EXPECT_OP(>=, val1, val2)
#define Z_EXPECT_LE(val1, val2) Z_EXPECT_OP(<=, val1, val2)
#define Z_EXPECT_GT(val1, val2) Z_EXPECT_OP(>, val1, val2)
#define Z_EXPECT_LT(val1, val2) Z_EXPECT_OP(<, val1, val2)

//expect two double's equality, a little different from integers
#define Z_EXPECT_DOUBLE_EQ(val1, val2)                                    \
    do                                                                    \
    {                                                                     \
        if ((val1) < (val2) - 0.001 || (val1) > (val2) + 0.001)           \
        {                                                                 \
            throw std::runtime_error("[Check failed] "#val1" == "#val2);  \
        }                                                                 \
    } while (0);
//expect two c-strings equality
#define Z_EXPECT_CSTR_EQ(val1, val2)                                          \
    do                                                                        \
    {                                                                         \
        if (strcmp((val1), (val2) != 0))                                      \
        {                                                                     \
            throw std::runtime_error("[Check failed] cstreq: "#val1", "#val2);\
        }                                                                     \
    } while (0);

#include <vector>
//declare the test class header
#define Z_BEGIN_TEST_CLASS(name)                                         \
    class ZTest##name                                                    \
    {                                                                    \
    public:                                                              \
        typedef void (ZTest##name::*TestCase)();                         \
        void RegisterTestCase(TestCase testcase)                         \
        {                                                                \
            m_testcases.push_back(testcase);                             \
        }                                                                \
    private:                                                             \
        std::vector<TestCase> m_testcases;                               \
    public:                                                              \
        void RunAllTest()                                                \
        {                                                                \
            fprintf(stderr, "\n--------------------------------------"); \
            fprintf(stderr, "--------------------------------------\n"); \
            fprintf(stderr, "Begin running test cases of %s:\n", #name); \
            unsigned int size = m_testcases.size();                      \
            int count = size;                                            \
            for (unsigned int i = 0; i < size; ++ i)                     \
            {                                                            \
                try                                                      \
                {                                                        \
                    fprintf(stderr, "[%d] ", i);                         \
                    (this->*m_testcases[i])();                           \
                }                                                        \
                catch (const std::runtime_error & error)                 \
                {                                                        \
                    fprintf(stderr, " %s\n", error.what());              \
                    -- count;                                            \
                }                                                        \
            }                                                            \
            fprintf(stderr, "Tatal %u cases, passed %d\n", size, count); \
        }
//delcare the test case        
#define Z_DECLARE_TEST_CASE(a)                                          \
    public:                                                             \
        void TestCase##a()                                              \
        {                                                               \
            fprintf(stderr, "Running test case: %s \t", #a);            \
            Run##a();                                                   \
            fprintf(stderr, " [Passed]\n");                             \
        }                                                               \
    private:                                                            \
        void Run##a();
//declare the test class end
#define Z_END_TEST_CLASS()                                              \
    };

//define a instance of the test class used to do the testing
#define Z_DEFINE_TEST_OBJECT(name, obj) ZTest##name obj;
//define a test case
#define Z_DEFINE_TEST_CASE(name, obj, a)                            \
    class ZTestCase##a                                              \
    {                                                               \
    public:                                                         \
        ZTestCase##a(ZTest##name & tester)                          \
        {                                                           \
            tester.RegisterTestCase(&ZTest##name::TestCase##a);     \
        }                                                           \
    };                                                              \
    static ZTestCase##a g_test_##name##_case_##a(obj);              \
    void ZTest##name::Run##a()
//run all the defined test cases
#define Z_RUN_ALL_TESTCASES(obj) obj.RunAllTest();

//if the test class need to access a class's protected or private members,
//you should define Z_NEED_ACCESS_PRIVATE_MEMBER before you include ztest.h,
//and then use Z_DECLARE_FRIEND to declare the test class to be a friend of
//the class to be tested
#ifdef Z_NEED_ACCESS_PRIVATE_MEMBER
#define Z_DECLARE_FRIEND(name) friend class ZTest##name;
#else
#define Z_DECLARE_FRIEND(name)
#endif //Z_NEED_ACCESS_PRIVATE_MEMBER

#endif //_XIAO5GEPROJECT_ZTEST_H_

