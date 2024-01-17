#pragma once

#include <queue>
#include <deque>
#include <afx.h>
#include <functional>

using namespace std;

class RecursiveMutex {
public:
	RecursiveMutex() {
		InitializeCriticalSection(&section_);
	}
	~RecursiveMutex() {
		DeleteCriticalSection(&section_);
	}
	PCRITICAL_SECTION cs() {
		return &section_;
	}
private:
	CRITICAL_SECTION section_;
};

class LockGuard {
public:
	LockGuard(RecursiveMutex& mutex) : mutex_(mutex) {
		EnterCriticalSection(mutex_.cs());
	}
	~LockGuard() {
		LeaveCriticalSection(mutex_.cs());
	}
private:
	RecursiveMutex& mutex_;
};

typedef std::function<void(void)> Task;

class TaskProcessor
{
public:
	enum Direction
	{
		Direction_Back,
		Direction_Front
	};

	TaskProcessor();

	~TaskProcessor();

	void AddTask(Task task);

	bool Start();

	void Stop();

	void Clear();

private:
	void Run();

	static unsigned int WINAPI RunThread(LPVOID lpVoid);

	void ProcessOnce();

	void CloseSingleThread(HANDLE& handle, const DWORD& time = 3000);

	std::queue<Task>		tasks_;

	RecursiveMutex			mutex_;

	bool					quit_;

	HANDLE					thread_;
};