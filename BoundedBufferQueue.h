/*********************************************************************
* 版权所有 (C)2016, 中兴仪器（深圳）有限公司。
*
* 文件名称：  BoundedBufferQueue
* 文件标识： 
* 内容摘要：  循环缓存加锁队列
* 其它说明：
* 当前版本：  V02.01.01
* 作 者：    赵栋
* 完成日期：  2016年9月10日
*
* 修改日期    版本号     修改人     修改内容
* ---------------------------------------------------------------
* 
******************************************************************/
#pragma once

#include <boost/circular_buffer.hpp>
#include <boost/thread/condition.hpp>
#include <boost/call_traits.hpp>
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>

/**************************************************************************
* 类声明 *
**************************************************************************/

NAMESPACEBEGIN

template <class T>
class BoundedBufferQueue
{
public:

	typedef boost::circular_buffer<T> container_type;
	typedef typename container_type::size_type size_type;
	typedef typename container_type::value_type value_type;
	typedef typename boost::call_traits<value_type>::param_type param_type;

	explicit BoundedBufferQueue(size_type capacity) : m_unread(0), m_container(capacity) {}
	explicit BoundedBufferQueue() : m_unread(0), m_container(1024) {}

	
	/*
	** push_front方法是 数据插入
	** 说明 插入到队头， 如队列满，将会等待 队列空的信号
	*/
	bool push_front(typename boost::call_traits<value_type>::param_type item)
	{ 
		boost::mutex::scoped_lock lock(m_mutex);
		m_not_full.wait(lock, boost::bind(&BoundedBufferQueue<value_type>::is_not_full, this));		// 判定队列是否满
		m_container.push_front(item);		// 数据插入队列
		++m_unread;
		lock.unlock();
		m_not_empty.notify_one();
		return true;
	}

	/*
	** pop_back方法是 数据出队列
	** 说明 队尾出队列
	*/
	bool pop_back(value_type* pItem) 
	{
		boost::mutex::scoped_lock lock(m_mutex);
		if( m_unread == 0 )
		{
			lock.unlock();
			m_not_full.notify_one();
			return false;
		}
		m_not_empty.wait(lock, boost::bind(&BoundedBufferQueue<value_type>::is_not_empty, this));	// 判定队列是否空
		*pItem = m_container[--m_unread];	// 数据出队列
		m_container.pop_back();
		lock.unlock();
		m_not_full.notify_one();
		return true;
	}

	/*
	** clear方法是 清空数据
	** 说明 将所有数据清空
	*/
	bool clear() 
	{
		boost::mutex::scoped_lock lock(m_mutex);
		m_container.clear();
		m_unread = 0;
		m_not_full.notify_one();
		lock.unlock();
		return true;
	}

	size_type length()
	{
		return m_unread;
	}

private:
//	BoundedBufferQueue(const bounded_buffer&){}      
//	BoundedBufferQueue& operator = (const bounded_buffer&){ return NULL;}

	// 队列是否为空
	bool is_not_empty() const { return m_unread > 0; }
	// 队列是否满
	bool is_not_full() const { return m_unread < m_container.capacity(); }

	size_type m_unread;				// 队列计数
	container_type m_container;		// 保存队列信息
	boost::mutex m_mutex;			// 互斥量
	boost::condition m_not_empty;	// 信号量 不空
	boost::condition m_not_full;	// 信号量 不满
};

// 默认 队列 大小 
const unsigned long queue_size = 1024L;

NAMESPACEEND
