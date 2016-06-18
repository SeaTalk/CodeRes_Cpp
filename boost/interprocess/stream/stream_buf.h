#ifndef _STREAM_BUF_H_
#define _STREAM_BUF_H_

#include <cstring>
#include <boost/interprocess/sync/interprocess_mutex.hpp> 
#include <boost/interprocess/sync/interprocess_condition.hpp> 
#include <boost/interprocess/sync/scoped_lock.hpp> 

#define STREAM_BUF_SIZE    4096

// 不是stdin那样阻塞，应该看做是文件stream
// 同步是必须的
struct StreamBuf {
    StreamBuf() : hasData(false)
    { memset(buf, 0, sizeof(buf)); }

    char buf[STREAM_BUF_SIZE];
    bool hasData;
    boost::interprocess::interprocess_mutex      lock;
    boost::interprocess::interprocess_condition  cond;
};



#endif

