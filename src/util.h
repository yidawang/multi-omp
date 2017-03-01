//
// Created by Yida Wang on 2/28/17.
//

#ifndef MULTI_OMP_UTIL_H
#define MULTI_OMP_UTIL_H
#include <stdio.h>
// ================================================================
// Utils
// ================================================================

// Refer to http://stackoverflow.com/a/39972671

inline std::string sformat(const char *fmt, ...) __attribute__ ((format (printf, 1, 2)));

inline std::string sformat(const char *fmt, ...)
{
    std::string result;

    va_list ap;
    va_start(ap, fmt);

    char *tmp = 0;
    vasprintf(&tmp, fmt, ap);
    va_end(ap);

    result = tmp;
    free(tmp);

    return result;
}
#endif //MULTI_OMP_UTIL_H
