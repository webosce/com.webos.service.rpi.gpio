//================================================
// Variables
//=================================================================================================
#include <stdio.h>
#include <glib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>


#include "../include/logging.h"

//=================================================================================================
// Functions
//=================================================================================================
void _good_assert(const char *cond_str, bool cond) {
    if (G_UNLIKELY(!(cond))) {
        GPIO_LOG_CRITICAL("ASSERTION_FAIL", 1, PMLOGKS("CAUSE", cond_str), "");
        *(int *)0x00 = 0;
    }
}

PmLogContext getServiceContext() {
    static PmLogContext logContext = 0;

    if (0 == logContext) {
        PmLogGetContext("gpioservice", &logContext);
    }

    return logContext;
}

// For output to kernel message.
#define LOG_BUF_MAX 512
void logKmsg(const char *fmt, ...) {
    char buf[LOG_BUF_MAX];
    va_list ap;
    int log_fd;

    log_fd = open("/dev/kmsg", O_WRONLY);

    if (log_fd < 0) return;

    va_start(ap, fmt);
    vsnprintf(buf, LOG_BUF_MAX, fmt, ap);
    buf[LOG_BUF_MAX - 1] = 0;
    va_end(ap);

    write(log_fd, buf, strlen(buf));
    close(log_fd);
}
