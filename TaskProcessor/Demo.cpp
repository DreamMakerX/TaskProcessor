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
	//����������ж���
	TaskProcessor taskProcessor;

	//�����������
	taskProcessor.Start();

	//����޲�����
	taskProcessor.AddTask(ShowSomething);

	//����в�����
	taskProcessor.AddTask(std::bind(Add, 1, 4));

	//����Person����
	Person person;
	person.age_ = 23;
	person.name_ = "zcc";

	//��Ӷ����޲�����
	taskProcessor.AddTask(std::bind(&Person::GetName, &person));

	//��Ӷ����в�����
	taskProcessor.AddTask(std::bind(&Person::SetAge, &person, 18));

	//�����ʱ��ȷ���������
	Sleep(100);

	//ֹͣ�������
	taskProcessor.Stop();

	std::cin.get();
}