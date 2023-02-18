//------------------------------------------------------------------------------
// pragma_demo.cpp
// 
// Author: https://www.youtube.com/watch?v=3jsqavQ5faU 
//------------------------------------------------------------------------------
//#define PRAGMA1

#ifdef PRAGMA1
#include <iostream>

void init();
void end();

#pragma(startup, init)
#pragma exit end

//------------------------------------------------------------------------------
// entry point
//------------------------------------------------------------------------------
int main() {

    std::cout << "main\n";
}

//------------------------------------------------------------------------------
// run before main()
//------------------------------------------------------------------------------
//void __attribute__((constructor)) init() {
void init() {

    std::cout << "init\n";
}

//------------------------------------------------------------------------------
// run after main()
//------------------------------------------------------------------------------
//void __attribute__((destructor)) end() {
void end() {

    std::cout << "end\n";
}
#else
// -----------------------------------------------------------------------------
// Initializer/finalizer sample for MSVC and GCC/Clang.
// 2010-2016 Joe Lowe. Released into the public domain.
// https://stackoverflow.com/questions/1113409/attribute-constructor-equivalent-in-vc
// -----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
#define INITIALIZER(f) \
        static void f(void); \
        struct f##_t_ { f##_t_(void) { f(); } }; static f##_t_ f##_; \
        static void f(void)
#elif defined(_MSC_VER)
#pragma section(".CRT$XCU",read)
#define INITIALIZER2_(f,p) \
        static void f(void); \
        __declspec(allocate(".CRT$XCU")) void (*f##_)(void) = f; \
        __pragma(comment(linker,"/include:" p #f "_")) \
        static void f(void)
#ifdef _WIN64
#define INITIALIZER(f) INITIALIZER2_(f,"")
#else
#define INITIALIZER(f) INITIALIZER2_(f,"_")
#endif
#else
#define INITIALIZER(f) \
        static void f(void) __attribute__((constructor)); \
        static void f(void)
#endif

static void finalize(void)
{
    printf("finalize\n");
}

INITIALIZER(initialize)
{
    printf("initialize\n");
    atexit(finalize);
}

int main(int argc, char** argv)
{
    printf("main\n");
    return 0;
}
#endif  // PRAGMA1
