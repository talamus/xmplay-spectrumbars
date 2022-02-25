#ifndef NDEBUG
#include "Debug.h"
namespace TN { namespace Debug {

/* Configuration */

const int COUNTERS_MAX       = 100;                 // Maximum number of interger counters
const FILE *OUTPUT_STREAM    = stderr;              // Output stream
const int TIMESTAMP_MAXLEN   = 40;                  // Timestamp buffer length (chars)
const char *TIMESTAMP_FORMAT = "%Y-%m-%d %H:%M:%S"; // Timestamp format
const char *LOG_FILENAME     = "tn_debug_log.txt";  // Debug log filename, written only if TN_DEBUG_LOG is #defined.

/* Function code */

void Die(const char *filename, int linenumber, const char *format, ...)
{
    #ifdef QT_CORE_LIB
        static QMutex mutex;
        QMutexLocker locker(&mutex);
    #endif
    va_list arg_ptr;
    Printf("\nAbnormal Exit:\n", filename, linenumber);
    va_start(arg_ptr, format);
    Vprintf(format, arg_ptr);
    va_end(arg_ptr);
    Printf(" (Program dies in '%s', line %d)\n", filename, linenumber);
    PrintCounters();
    exit(-1);
}

void Debug(const char *filename, int linenumber, const char *format, ...)
{
    #ifdef QT_CORE_LIB
        static QMutex mutex;
        QMutexLocker locker(&mutex);
    #endif
    va_list arg_ptr;
    va_start(arg_ptr, format);
    Vprintf(format, arg_ptr);
    va_end(arg_ptr);
    Printf(" (in '%s', line %d)\n", filename, linenumber);
}

const char *CounterNames[COUNTERS_MAX];
const int *CounterCurrent[COUNTERS_MAX];
int CounterMin[COUNTERS_MAX];
int CounterMax[COUNTERS_MAX];
int Counters = 0;

void Count(const char *name, const int &number)
{
    #ifdef QT_CORE_LIB
        static QMutex mutex;
        QMutexLocker locker(&mutex);
    #endif
    int i;
    int found = -1;
    for (i = Counters; found == -1 && i--;) {
        if (CounterCurrent[i] == &number) { found = i; }
    }
    if (found != -1) {
        CounterMin[found] = number < CounterMin[found] ? number : CounterMin[found];
        CounterMax[found] = number > CounterMax[found] ? number : CounterMax[found];
    } else if (Counters >= COUNTERS_MAX) {
        Die("Debug.cpp", 7, "Counter list is full! Raise the value of COUNTERS_MAX or disable debugging.");
    } else {
        CounterNames[Counters] = name;
        CounterCurrent[Counters] = &number;
        CounterMin[Counters] = number;
        CounterMax[Counters] = number;
        ++Counters;
    }
}

void PrintCounters()
{
    #ifdef QT_CORE_LIB
        static QMutex mutex;
        QMutexLocker locker(&mutex);
    #endif
    int i;
    if (Counters) {
        Printf("\n/== Counters ========================== Max == Current == Min ==\\\n");
        for (i = Counters; i--;) {
            Printf("| %-30s %10d %10d %6d   |\n", CounterNames[i], CounterMax[i], *CounterCurrent[i], CounterMin[i]);
        }
        Printf("\\---------------------------------------------------------------/\n\n");
    }
}

const char *GetTimestamp()
{
    #ifdef QT_CORE_LIB
        static QMutex mutex;
        QMutexLocker locker(&mutex);
    #endif
    static char timestamp[TIMESTAMP_MAXLEN];
    time_t now = time(0);
    const struct tm *tm = localtime(&now);
    strftime(timestamp, TIMESTAMP_MAXLEN, TIMESTAMP_FORMAT, tm);
    return timestamp;
}

void Printf(const char *format, ...)
{
    #ifdef QT_CORE_LIB
        static QMutex mutex;
        QMutexLocker locker(&mutex);
    #endif
    va_list arg_ptr;
    va_start(arg_ptr, format);
    Vprintf(format, arg_ptr);
    va_end(arg_ptr);
}

void Vprintf(const char *format, va_list arg_ptr)
{
    #ifdef QT_CORE_LIB
        static QMutex mutex;
        QMutexLocker locker(&mutex);
    #endif

    #ifdef TN_DEBUG_LOG
    static FILE *DebugLog = 0;
    if (!DebugLog) {
        DebugLog = fopen(LOG_FILENAME, "a+");
        if (DebugLog) {
            fprintf(DebugLog, "\n=== Debug Log =========================== %s ===\n\n", GetTimestamp());
            fprintf((FILE *) OUTPUT_STREAM, "Debug Log: Log written in file '%s'\n", LOG_FILENAME);
        } else {
            fprintf((FILE *) OUTPUT_STREAM, "Debug Log: Unable to open file '%s' for writing\n", LOG_FILENAME);
        }
    }
    if (DebugLog) vfprintf(DebugLog, format, arg_ptr);
    #endif // TN_DEBUG_LOG

    vfprintf((FILE *) OUTPUT_STREAM, format, arg_ptr);
}

int BatonState = 0;
const char Batons[] = {'-', '/', '|', '\\'};
void SwivelingBaton()
{
    printf("%c\b", Batons[BatonState++ % 4]);
}

int PreviousPercentage = -1;
void Percentage(double number, double maximum)
{
    int per = (int) (number/maximum*100.0);
    if (per != PreviousPercentage) {
        printf("% 3d%% \b\b\b\b\b", per);
        PreviousPercentage = per;
    }
}

char *DoubleToHexString(double n)
{
    static char buffer[sizeof(double)*2];  // questionable...

    union {
        double number;
        unsigned char bytes[sizeof(double)];
    } convert;

    convert.number = n;
    for (size_t i = 0; i < sizeof(double); ++i) {
        sprintf(&buffer[i*2], "%02x", (unsigned int) convert.bytes[i]);  // ugly!
    }

    return buffer;
}

double HexStringToDouble(const char *str)
{
    union {
        double number;
        unsigned char bytes[sizeof(double)*2];  // larger than required
    } convert;

    for (size_t i = 0; i < sizeof(double); ++i) {
        sscanf(str + i*2, "%02x", (unsigned int *) &convert.bytes[i]);  // double ugly!
    }

    return convert.number;
}

#ifdef QT_CORE_LIB
QTime *Stopwatch()
{
    static QTime stopwatch;
    return &stopwatch;
}
#endif

} } // namespace TN::Debug
#endif // NDEBUG

// EOF
