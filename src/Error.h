#ifndef _ERROR_H
#define _ERROR_H

typedef enum {
  INVALID_INSTRUCTION,
  BREAK_EXCEPTION,
  SLEEP_EXCEPTION,
  WATCHDOG_EXCEPTION,
} ErrorCode;

#endif // _ERROR_H
