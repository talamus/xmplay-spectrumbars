#ifndef TN_DEBUG_H
#define TN_DEBUG_H 0.2

/*
 * Simple debugging and surveillance tools, in semi-classic C style
 *
 * If NDEBUG is defined no code is generated
 * If TN_DEBUG_LOG is defined a debug log file will be written
 *
 * With QT all operations are thread safe! (Well... perhaps except the stopwatch.)
 */

#ifndef NDEBUG

    #include <stdio.h>
    #include <stdlib.h>
    #include <stdarg.h>
    #include <time.h>
    #ifdef QT_CORE_LIB
        #include <QTime>
        #include <QMutex>
        #include <QMutexLocker>
    #endif

    /* Macros, use these */

    #define ASSERT(test)    ((test) ? (void) 0 : DIE("Assertion (%s) failed", #test))  // Almost the classical assert()
    #define CHECK_PTR(ptr)  ((ptr) ? (void) 0 : DIE("Null pointer / Out of memory"))   // Assert for null pointer detection

    #define DEBUG(...)      (TN::Debug::Debug(__FILE__, __LINE__, __VA_ARGS__))   // Print a debug message (printf-style)
    #define DIE(...)        (TN::Debug::Die(__FILE__, __LINE__, __VA_ARGS__))     // Die for a good cause (printf-style)
    #define PRINTF(...)     (TN::Debug::Printf(__VA_ARGS__))                      // Just print (printf-style)

    #define NEW_COUNTER(label)         int label = 0;                             // Create a new counter
    #define EXTERN_COUNTER(label)      extern int label;                          // Use external counter
    #define SET_COUNTER(label, value)  (TN::Debug::Count(#label, label = value))  // Set counter value
    #define INCR_COUNTER(label)        (TN::Debug::Count(#label, ++label))        // Counter ++
    #define DECR_COUNTER(label)        (TN::Debug::Count(#label, --label))        // Counter --
    #define PRINT_COUNTERS()           (TN::Debug::PrintCounters())               // Print all counters

    #define BATON()                    (TN::Debug::SwivelingBaton())              // Show a swiveling baton to indicate that everything is all right!
    #define PERCENT(number, maximum)   (TN::Debug::Percentage(number, maximum))   // Show a throwaway percent number to indicate progress!

    #ifdef QT_CORE_LIB
        #define STOPWATCH_START()      (TN::Debug::Stopwatch()->start())          // Start timing task
        #define STOPWATCH_STOP()       DEBUG("Stopwatch stopped at %u ms", TN::Debug::Stopwatch()->elapsed())
    #endif                                                                        // Stop timing task

    /* Functions, do not use these */

    namespace TN { namespace Debug {
        void Debug(const char *filename, int linenumber, const char *message_with_prinf_format, ...);  // Print a debug message
        void Die(const char *filename, int linenumber, const char *message_with_prinf_format, ...);    // Die for a good cause
        void Count(const char *counter_label, const int &counter);   // Watch and remember the counter value
        void PrintCounters();                                        // Print all counters
        const char *GetTimestamp();                                  // Return pointer to a timestamp string
        void SwivelingBaton();                                       // Swivel that baton!
        void Percentage(double number, double maximum);              // Print a percentage
        void Printf(const char *format, ...);                        // Printf, calls Vprintf
        void Vprintf(const char *format, va_list arg_ptr);           // Vprintf, all output goes thru this function
        char *DoubleToHexString(double number);                      // Convert double to raw hex string
        double HexStringToDouble(const char *double_as_hex_string);  // Convert raw hex string to double
        #ifdef QT_CORE_LIB
            QTime *Stopwatch();                                      // Give me a stopwatch
        #endif
    } } // namespace TN::Debug

#else // NDEBUG

    /* All debug is turned off! */

    #define ASSERT(...)
    #define CHECK_PTR(...)
    #define DEBUG(...)
    #define DIE(...)
    #define PRINTF(...)
    #define NEW_COUNTER(...)
    #define EXTERN_COUNTER(...)
    #define SET_COUNTER(...)
    #define INCR_COUNTER(...)
    #define DECR_COUNTER(...)
    #define PRINT_COUNTERS(...)
    #define BATON(...)
    #define PERCENT(...)
    #define STOPWATCH_START(...)
    #define STOPWATCH_STOP(...)

    #warning ###########################
    #warning ###*********************###
    #warning ###*** DEBUG is OFF! ***###
    #warning ###*********************###
    #warning ###########################

#endif // NDEBUG
#endif // TN_DEBUG_H
