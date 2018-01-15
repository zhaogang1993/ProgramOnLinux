#ifdef __cplusplus
extern "C"
{
#endif

#ifndef BASE_TYPE_H
#define BASE_TYPE_H

#define AVERAGE(a, b)  ((a&b) + ((a^b)>>1))
#define SUM(a, b)      (((a&b) + ((a^b)>>1))<<1)

/* 是否是2的N次方 */
#define ISPOWER2(a)  (((a)&((a)-1)) == 0)

#ifndef MAX
    #define MAX(a,b) ((a)>(b)?(a):(b))
#endif

#ifndef MIN
    #define MIN(a,b) ((a)>(b)?(b):(a))
#endif

#define STATIC static
#define INLINE inline 
#define DOUBLE double
#define UCHAR unsigned char
#define CHAR char
#define UINT unsigned int
#define INT int
#define ULONG unsigned long
#define LONG long
#define USHORT unsigned short
#define SHORT short
#define VOID void 
#define FLOAT float

#define BUF_LEN_8  (8)
#define BUF_LEN_16 (16)
#define BUF_LEN_32 (32)
#define BUF_LEN_64 (64)
#define BUF_LEN_128 (128)
#define BUF_LEN_256 (256)
#define BUF_LEN_512 (512)

#define BUF_LEN_1K (1024)
#define BUF_LEN_10K     ((10)*BUF_LEN_1K)

#define BUF_LEN_20      (20)
#define BUF_LEN_100     (100)

#define BUF_LEN_IPv4     (16)       /* IPv4字符串长 */

#ifndef BOOL_T
    typedef USHORT BOOL_T;
#endif

#define CODE_SPLIT(src)

#define BOOL_TRUE  ((BOOL_T)1)
#define BOOL_FALSE ((BOOL_T)0)

#define DEFINE_ERROR_TYPE(base, num)        ((base) + (num))
#define ERROR_SUCCESS           0x00000000UL
#define ERROR_FAILED            DEFINE_ERROR_TYPE(ERROR_SUCCESS, 1)
#define ERROR_NOT_EXIST         DEFINE_ERROR_TYPE(ERROR_SUCCESS, 2)
#define ERROR_NOT_SUPPORT       DEFINE_ERROR_TYPE(ERROR_SUCCESS, 3)

#define LPVOID void *
#define HANDLE ULONG

typedef long long __attribute__((aligned(8)))  INT64;
typedef unsigned long long __attribute__((aligned(8)))  UINT64;

#ifndef ARRAY_SIZE
    #define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))
#endif

#define UINT64_L32(UINT64_src) (UINT)(((UINT64_src) & 0x00000000FFFFFFFF))
#define UINT64_H32(UINT64_src) (UINT)((UINT64)((UINT64_src) & 0xFFFFFFFF00000000)>>32)
#define UINT64_FILL(H32,L32) (UINT64)((UINT64)((UINT64)(H32) << 32) | (L32))

#define UINT_GET_H8(_uiData) (UINT)(((_uiData) & 0xFF000000)>>24)
#define UINT_GET_L8(_uiData) (UINT)(((_uiData) & 0x000000FF))
#define UINT_GET_H16(_uiData) (UINT)(((_uiData) & 0xFFFF0000)>>16)
#define UINT_GET_L16(_uiData) (UINT)(((_uiData) & 0x0000FFFF))

#define UINT_GET_HH8(_uiData) (UINT)(((_uiData) & 0xFF000000)>>24)
#define UINT_GET_HL8(_uiData) (UINT)(((_uiData) & 0x00FF0000)>>16)
#define UINT_GET_LH8(_uiData) (UINT)(((_uiData) & 0x0000FF00)>>8)
#define UINT_GET_LL8(_uiData) (UINT)((_uiData) & 0x000000FF)

#define BIT_TO_UINT(_byte1,_byte2,_byte3,_byte4) \
    ((UINT)(((_byte1)<< 24) | ((_byte2)<<16) | ((_byte3)<<8) | (_byte4)))


#ifndef IN
    #define IN
#endif

#ifndef OUT
    #define OUT
#endif

/* #ifndef CODE_SPLIT */
    /* #define CODE_SPLIT */
/* #endif */

#ifndef INOUT
    #define INOUT
#endif

#ifndef CONST
    #define CONST const
#endif /* CONST */

#define INVALID_MSG_ID              0xffffffff

#define MAC_ADDR_LEN     6

/* 宏参数字符串化 */
#define  STRINGIFY(x) #x 
#define  TOSTRING(x) STRINGIFY(x) 

#define  _FILE_LINE __FILE__ ":" TOSTRING(__LINE__) 

/* 宏参数字符串拼接 */
#define STR_APPEND2_STR(param1, param2)  TOSTRING(param1##param2)

#define ZHAOGANG_LOG_FILE "/home/zhaogang/share/code/output/zhao.log"
#define ZHAOGANG_FIFO_FILE      "/tmp/fifo_test"
#define ZHAOGANG_POSIXMQ_FILE   "/posix_mq"         /* POSIX消息队列名称有限制 */

#define FILE_777_MODE       (S_IRUSR | S_IWUSR | S_IXUSR | \
                             S_IRGRP | S_IWGRP | S_IXGRP | \
                             S_IROTH | S_IWOTH | S_IXOTH)

#define SockAddr_S struct sockaddr
#define IfConf_S  struct ifconf

#define ZHAOGANG_SERVER_NAME     "zhaod"
#define ZHAOGANG_CLIENT_NAME     "clientd"
#define ZHAOGANG_INVALID_FD     (-1)
#define ZHAOGANG_MAX_EPOLL_EVENT   10

#define PUBLIC_BASE_PORT            3000
#define DEFINE_PUBLIC_PORT(base, index)\
        ((base) + (index))

#define SERVER_TCP_PORT          DEFINE_PUBLIC_PORT(PUBLIC_BASE_PORT, 0)
#define SERVER_UDP_PORT          DEFINE_PUBLIC_PORT(PUBLIC_BASE_PORT, 1)
#define SERVER_SCTP_PORT         DEFINE_PUBLIC_PORT(PUBLIC_BASE_PORT, 2)
#define SERVER_HTTP_PORT         DEFINE_PUBLIC_PORT(PUBLIC_BASE_PORT, 3)
#define SERVER_BROADCAST_PORT    DEFINE_PUBLIC_PORT(PUBLIC_BASE_PORT, 4)
#define SERVER_MUTILCAST_PORT    DEFINE_PUBLIC_PORT(PUBLIC_BASE_PORT, 5)
#define SERVER_SYNIO_PORT        DEFINE_PUBLIC_PORT(PUBLIC_BASE_PORT, 6)
#define SERVER_OPENCVD_TCP_PORT  DEFINE_PUBLIC_PORT(PUBLIC_BASE_PORT, 7)

#define BROADCAST_ADDR_STR       "255.255.255.255"
#define MUTILCASE_ADDR_STR       "224.0.0.100"
#define LOOPBACK_ADDR_STR        "127.0.0.1"
#define LOOPBACK_ADDR            0x7f00000001UL
#define UNIXDOMAIN_PATH          "/tmp/unixun_sock"

#endif

#ifdef __cplusplus
}
#endif
