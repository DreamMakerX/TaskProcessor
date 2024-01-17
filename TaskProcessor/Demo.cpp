#include "TaskProcessor.h"
#include <iostream>

void ShowSomething(){
	std::cout << "ShowSomething" << std::endl;
}

void Add(int first, int second){
	std::cout << "Add: " << first + second << std::endl;
}

class Person
{
public:
	int age_;
	std::string name_;

	void GetName() {
		std::cout << "GetName: " << name_.c_str() << std::endl;
	}
	void SetAge(int age) {
		age_ = age;
		std::cout << "SetAge: " << age << std::endl;
	}
};

void main()
{
	//声明任务队列对象
	TaskProcessor taskProcessor;

	//启动任务队列
	taskProcessor.Start();

	//添加无参任务
	taskProcessor.AddTask(ShowSomething);

	//添加有参任务
	taskProcessor.AddTask(std::bind(Add, 1, 4));

	//创建Person对象
	Person person;
	person.age_ = 23;
	person.name_ = "zcc";

	//添加对象无参任务
	taskProcessor.AddTask(std::bind(&Person::GetName, &person));

	//添加对象有参任务
	taskProcessor.AddTask(std::bind(&Person::SetAge, &person, 18));

	//添加延时，确保任务完成
	Sleep(100);

	//停止任务队列
	taskProcessor.Stop();

	std::cin.get();
}