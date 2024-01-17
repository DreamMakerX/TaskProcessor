#include "TaskProcessor.h"
#include <process.h> 

const unsigned int _executeCycle = 10;	//任务执行周期，ms 
const unsigned int _maxQueueSize = 300;	//任务队列最大值 

TaskProcessor::TaskProcessor()
	: thread_(INVALID_HANDLE_VALUE)
{}

TaskProcessor::~TaskProcessor()
{
	Stop();
}

void TaskProcessor::AddTask(Task task)
{
	LockGuard lock(mutex_);
	if(tasks_.size() < _maxQueueSize)
	{
		tasks_.push(task);
	}
}

bool TaskProcessor::Start()
{
	if (thread_ != INVALID_HANDLE_VALUE)
	{
		return true;
	}

	quit_ = false;

	unsigned thread_id;
	thread_ = (HANDLE)_beginthreadex(NULL, 0, &RunThread, (LPVOID)this, 0, &thread_id);

	if (thread_ == 0 || thread_ == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	return true;
}

void TaskProcessor::Stop()
{
	quit_ = true;
	CloseSingleThread(thread_);
}

void TaskProcessor::Clear()
{
	LockGuard lock(mutex_);

	std::queue<Task> tmp;
	tasks_.swap(tmp);
}

void TaskProcessor::Run()
{
	while (!quit_)
	{
		Sleep(_executeCycle);
		ProcessOnce();
	}
}

unsigned int WINAPI TaskProcessor::RunThread(LPVOID lpVoid)
{
	TaskProcessor* task_processor = (TaskProcessor*)lpVoid;
	task_processor->Run();
	return 0;
}

void TaskProcessor::ProcessOnce()
{
	Task task;

	{
		LockGuard lock(mutex_);

		if (!tasks_.empty())
		{
			task = tasks_.front();

			tasks_.pop();

			task();
		}
	}

}

void TaskProcessor::CloseSingleThread(HANDLE& handle, const DWORD& time)
{
	if (handle != INVALID_HANDLE_VALUE)
	{
		WaitForSingleObject(handle, time);
		CloseHandle(handle);

		handle = INVALID_HANDLE_VALUE;
	}
}
