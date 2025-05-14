#include "Log.h"

int main(void) {
    Log *log = Log::Instance(3);
    log->message(LOG_NORMAL, "program loaded 0");
    log->message(LOG_NORMAL, "program loaded 1");
    log->message(LOG_WARNING, "program loaded 2");
    log->message(LOG_NORMAL, "program loaded 3");
    log->message(LOG_NORMAL, "program loaded 4");
    log->message(LOG_NORMAL, "program loaded 5");
    log->message(LOG_ERROR, "error happens! help me!6");
    log->print();
    Log *log2 = Log::Instance(5); // cant
    delete log;
}
