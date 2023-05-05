#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <inttypes.h>

#define DEBUG_ON 1

#define LOG_ON 1
#define LOG(fmt, ...) \
        do { if (LOG_ON) fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
                                __LINE__, __func__, __VA_ARGS__); } while (0)

#endif

struct __attribute__((__packed__)) msg_header {
    uint64_t msg_len;
    uint16_t msg_type;
};

struct __attribute__((__packed__)) msg_task {
        struct msg_header header;
        char block[128];
        long difficulty;
};

struct __attribute__((__packed__)) msg_solution {
        struct msg_header header;
        char username[20];
        uint64_t nonce;
};

union __attribute__((__packed__)) msg_wrapper {
        struct msg_header header;
        struct msg_task task;
        struct msg_solution solution;
};

enum MSG_TYPES {
        MSG_SOLUTION,
        MSG_VERIFICATION,
        MSG_HEARTBEAT,
        MSG_STATUS,
        MSG_ID,
        MSG_TASK,
};

size_t msg_size(enum MSG_TYPES type);

/**
 * Function: read_len
 * Purpose:  reads from an input stream, retrying until a specific number of
 *           bytes has been read. This ensures complete message delivery.
 *
 * Args:
 *  * fd     - the file descriptor to read from
 *  * buf    - pointer to buffer to store data
 *  * length - size of the incoming message. If less than 'length' bytes are
 *             received, we'll keep retrying the read() operation.
 */
int read_len(int fd, void *buf, size_t length);

int write_len(const int fd, const void *buf, size_t length);

union msg_wrapper create_msg(enum MSG_TYPES type);

union msg_wrapper read_msg(int fd);

int write_msg(int fd, const union msg_wrapper *msg);