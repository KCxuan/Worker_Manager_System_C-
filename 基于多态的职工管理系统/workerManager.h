#pragma once//防止头文件重复包含
#include <iostream>
#include <fstream>
#include"worker.h"
#define FILENAME "empFile.txt"
using namespace std;

class WorkerManager {
public:
	//构造函数
	WorkerManager();
	//展示菜单 
	void Show_Menu();
	//增加员工
	void Add_Emp();
	//保存文件
	void Save();
	//统计人数
	int get_EmpNum();
	//初始化员工
	void init_Emp();
	//显示职工
	void Show_Emp();
	//删除职工
	void Del_Emp();
	//修改职工
	void Mod_Emp();
	//判断职工是否存在
	int IsExist(int id);
	//查找职工
	void Find_Emp();
	//排序职工
	void Sort_Emp();
	//清空文档
	void Clean_File();
	//退出系统
	void Exitsystem();
	//析构函数
	~WorkerManager();
	//记录职工人数
	int m_Empnum;
	//职工数组的指针
	Worker** m_EmpArray;
	//标志文件是否为空
	bool m_FileEmpty;
};

