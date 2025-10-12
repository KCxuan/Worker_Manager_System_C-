#include "workerManager.h"
#include "worker.h"
#include "employee.h"
#include "manager.h"
#include "boss.h"
#include <algorithm>


WorkerManager::WorkerManager() {
	ifstream ifs;
	ifs.open(FILENAME, ios::in);//打开文件
	//文件不存在的情况
	if (!ifs.is_open()) {
		//cout << "文件不存在" << endl;
		this->m_Empnum = 0;//一开始没有人
		this->m_EmpArray = NULL;//初始化数组
		this->m_FileEmpty = true;//初始化文件为空标志
		ifs.close();//关闭文件
		return;
	}
	//文件存在但为空的情况
	char ch;
	ifs >> ch;
	if (ifs.eof()) {
		//cout << "文件为空" << endl;
		this->m_Empnum = 0;//一开始没有人
		this->m_EmpArray = NULL;//初始化数组
		this->m_FileEmpty = true;//初始化文件为空标志
		ifs.close();//关闭文件
		return;
	}
	//文件存在且不为空的情况
	this->m_Empnum = this->get_EmpNum();
	//--------------------------------------
	this->m_FileEmpty = false;//文件不为空
	//--------------------------------------
	//cout << "职工人数为：" << this->m_Empnum << endl;
	//开辟空间
	this->m_EmpArray = new Worker * [this->m_Empnum];
	//将文件中的数据存到数组中
	init_Emp();
}

//删除职工
void WorkerManager::Del_Emp() {
	if (this->m_FileEmpty) {
		cout << "文件不存在或记录为空！" << endl;
	}
	else {
		//按职工编号删除
		cout << "请输入想要删除的职工编号：" << endl;
		int id = 0;
		cin >> id;
		int index = this->IsExist(id);
		if (index != -1) {
			//找到编号，删除index上的数字
			//删除操作,将后面的数据往前移
			for (int i = index; i < this->m_Empnum - 1; i++) {
				this->m_EmpArray[i] = this->m_EmpArray[i + 1];
			}
			this->m_Empnum--;//更新职工人数
			if(this->m_Empnum==0){
				this->m_FileEmpty = true;
				//职工数组指向空
				//delete[] this->m_EmpArray;
				//this->m_EmpArray = NULL;
			}
			//将数据同步到文件中
			this->Save();
			cout << "删除成功" << endl;
		}
		else {
			cout << "删除失败，未找到该职工" << endl;
		}
	}
	//按任意键清屏，返回上级目录
	system("pause");
	system("cls");
}
//判断职工是否存在
int WorkerManager::IsExist(int id) {
	int index = -1;
	for (int i = 0; i < this->m_Empnum; i++) {
		if(this->m_EmpArray[i]->m_id == id){
			index = i;
			break;
		}
	}
	return index;
}

int WorkerManager::get_EmpNum() {
	ifstream ifs;
	ifs.open(FILENAME, ios::in);//打开文件,读取

	int id;
	string name;
	int dId;

	int num = 0;

	while (ifs >> id && ifs >> name && ifs >> dId) {
		//记录人数
		num++;
	}
	ifs.close();

	return num;
}

WorkerManager::~WorkerManager() {
	if (this->m_EmpArray != NULL) {
		delete[] this->m_EmpArray;
		this->m_EmpArray = NULL;
	}
}
//显示职工
void WorkerManager::Show_Emp() {
	if (this->m_FileEmpty) {
		cout << "文件不存在或记录为空！" << endl;
	}
	else {
		for (int i = 0; i < this->m_Empnum; i++) {
			//利用多态调用接口
			this->m_EmpArray[i]->showInfo();
		}
	}
	system("pause");
	system("cls");
}


//初始化员工
void WorkerManager::init_Emp() {
	ifstream ifs;
	ifs.open(FILENAME, ios::in);//打开文件,读取
	int id;
	string name;
	int dId;
	int index = 0;
	while (ifs >> id && ifs >> name && ifs >> dId) {
		Worker* worker = NULL;
		if (dId == 1) {
			worker = new Employee(id, name, dId);
		}
		else if (dId == 2) {
			worker = new Manager(id, name, dId);
		}
		else {
			worker = new Boss(id, name, dId);
		}
		this->m_EmpArray[index] = worker;
		index++;
	}
	ifs.close();
}

void WorkerManager::Show_Menu() {
	cout << "********************************" << endl;
	cout << "******欢迎使用职工管理系统******" << endl;
	cout << "*********0.退出管理系统*********" << endl;
	cout << "*********1.增加职工信息*********" << endl;
	cout << "*********2.显示职工信息*********" << endl;
	cout << "*********3.删除离职职工*********" << endl;
	cout << "*********4.修改职工信息*********" << endl;
	cout << "*********5.查找职工信息*********" << endl;
	cout << "*********6.按照编号排序*********" << endl;
	cout << "*********7.清空所有文档*********" << endl;
	cout << "********************************" << endl;
	cout << endl;
}

void WorkerManager::Save() {
	ofstream ofs;
	ofs.open(FILENAME, ios::out);//创建文件
	//将每个人的数据写入到文件中
	for (int i = 0; i < this->m_Empnum; i++) {
		ofs << this->m_EmpArray[i]->m_id << " "
			<< this->m_EmpArray[i]->m_Name << " "
			<< this->m_EmpArray[i]->m_DeptId << endl;
	}
	//关闭文件
	ofs.close();
}

void WorkerManager::Add_Emp() {
	cout << "请输入要添加的员工数量" << endl;
	int addnum = 0;
	cin >> addnum;
	if (addnum > 0) {
		//计算新员工的总人数
		int newSize = this->m_Empnum + addnum;
		//在堆区创建指针数组，并返回指针数组的指针
		Worker** newSpace = new Worker*[newSize*sizeof(Worker)];
		if (this->m_EmpArray != NULL) {
			//如果原来有数据，将其拷贝到新的指针数组里
			for (int i = 0; i < this->m_Empnum; i++) {
				newSpace[i] = this->m_EmpArray[i];
			}
		}
		//开始将新的职工信息添加到数组里
		for (int i = 0; i < addnum; i++) {
			int id;
			string name;
			int dSelect;

			cout << "请输入第 " << i + 1 << "个新职工编号：" << endl;
			cin >> id;

			cout << "请输入第 " << i + 1 << "个新职工姓名：" << endl;
			cin >> name;

			cout << "请选择员工的岗位:" << endl;
			cout << "1:普通职工" << endl;
			cout << "2:经理" << endl;
			cout << "3:老板" << endl;
			cin >> dSelect;
			//创建职工指针
			Worker* worker = NULL;

			switch (dSelect) {
			case 1:
				worker = new Employee(id, name, 1);
				break;
			case 2:
				worker = new Manager(id, name, 2);
				break;
			case 3:
				worker = new Boss(id, name, 3);
				break;
			default:
				break;
			}
			newSpace[this->m_Empnum + i] = worker;
		}
		//释放原来指针的内存
		delete[] this->m_EmpArray;
		//将指针指向新的地方
		this->m_EmpArray = newSpace;

		this->m_Empnum = newSize;
		//更新文件不为空的标志
		this->m_FileEmpty = false;
		
		cout << "成功添加" << addnum << "个新职工" << endl;
		//保存到文件中
		this->Save();
	}else {
		cout << "输入有误！" << endl;
	}
	//按任意键清屏，返回上级目录
	system("pause");
	system("cls");
}

//修改职工
void WorkerManager::Mod_Emp() {
	if (this->m_FileEmpty) {
		cout << "文件不存在或记录为空！" << endl;
	}
	else {
		cout << "请输入想要修改的职工编号：" << endl;
		int id = 0;
		cin >> id;
		int ret = this->IsExist(id);
		if (ret != -1) {
			//找到编号，修改index上的数字
			delete this->m_EmpArray[ret];
			int newId = 0;
			string newName = "";
			int dSelect = 0;
			cout << "查到：" << id << "号职工，请输入新职工号：" << endl;
			cin >> newId;
			cout << "请输入新姓名：" << endl;
			cin >> newName;
			cout << "请输入岗位：" << endl;
			cout << "1:普通职工" << endl;
			cout << "2:经理" << endl;
			cout << "3:老板" << endl;
			cin >> dSelect;
			Worker* worker = NULL;
			if (dSelect == 1) {
				worker = new Employee(newId, newName, 1);
			}
			else if (dSelect == 2) {
				worker = new Manager(newId, newName, 2);
			}
			else if(dSelect == 3){
				worker = new Boss(newId, newName, 3);
			}
			//更新数组
			this->m_EmpArray[ret] = worker;

			cout << "修改成功" << endl;
			//保存到文件中
			this->Save();
		}
		else {
			cout << "修改失败，未找到该职工" << endl;
		}
	}
	//按任意键清屏，返回上级目录
	system("pause");
	system("cls");
}

void WorkerManager::Find_Emp() {
	if (this->m_FileEmpty) {
		cout << "文件不存在或记录为空！" << endl;
	}
	else {
		cout << "请输入想要查找的方式：" << endl;
		cout << "1.按职工编号查找" << endl;
		cout << "2.按职工姓名查找" << endl;
		int select = 0;
		cin >> select;
		if (select == 1) {
			//按编号查找
			int id;
			cout << "请输入职工编号：" << endl;
			cin >> id;
			int ret = this->IsExist(id);
			if (ret != -1) {
				cout << "查找成功，职工信息如下：" << endl;
				this->m_EmpArray[ret]->showInfo();
			}
			else {
				cout << "查找失败，未找到该职工" << endl;
			}
		}
		else if (select == 2) {
			//按姓名查找
			string name;
			cout << "请输入职工姓名：" << endl;
			cin >> name;
			bool flag = false;//是否找到
			for (int i = 0; i < this->m_Empnum; i++) {
				if (this->m_EmpArray[i]->m_Name == name) {
					cout << "查找成功，职工信息如下：" << endl;
					this->m_EmpArray[i]->showInfo();
					flag = true;
				}
			}
			if (!flag) {
				cout << "查找失败，未找到该职工" << endl;
			}
		}
		else {
			cout << "输入选项有误，请重新输入" << endl;
		}
	}
	system("pause");
	system("cls");
}

//排序职工 
void WorkerManager::Sort_Emp() {
	if (this->m_FileEmpty) {
		cout << "文件不存在或记录为空！" << endl;
	}
	else {
		cout << "请选择排序方式：" << endl;
		cout << "1.按职工编号升序" << endl;
		cout << "2.按职工编号降序" << endl;
		int select = 0;
		cin >> select;
		if (select == 1) {
		//升序
			for (int i = 0; i < this->m_Empnum - 1; i++) {
				for (int j = 0; j < this->m_Empnum - 1 - i; j++) {
					if (this->m_EmpArray[j]->m_id > this->m_EmpArray[j + 1]->m_id) {
						Worker* temp = this->m_EmpArray[j];
						this->m_EmpArray[j] = this->m_EmpArray[j + 1];
						this->m_EmpArray[j + 1] = temp;
					}
				}
			}
		}
		else {
			//降序
			sort(this->m_EmpArray, this->m_EmpArray + this->m_Empnum, [](Worker* a, Worker* b) {
				return a->m_id > b->m_id;
			});
		}
		cout << "排序成功，排序后结果为：" << endl;
		this->Show_Emp();
		//将数据保存到文件中
		this->Save();
	}
	system("pause");
	system("cls");
}

void WorkerManager::Clean_File() {
	cout << "确认清空？" << endl;
	cout << "1.确认" << endl;
	cout << "2.取消" << endl;
	int select = 0;
	cin >> select;
	if (select == 1) {
		//打开模式ios::trunc 如果存在删除文件并重新创建
		ofstream ofs(FILENAME, ios::trunc);
		ofs.close();
		//删除堆区数据
		if (this->m_EmpArray != NULL) {
			for (int i = 0; i < this->m_Empnum; i++) {
				//释放每个职工对象
				if (this->m_EmpArray[i] != NULL) {
					delete this->m_EmpArray[i];
					this->m_EmpArray[i] = NULL;
				}
			}
			//释放数组指针
			delete[] this->m_EmpArray;
			this->m_EmpArray = NULL;
			//初始化成员属性
			this->m_Empnum = 0;
			//文件为空标志
			this->m_FileEmpty = true;
		}
		cout << "清空成功！" << endl;
	}
	else {
		cout << "取消清空！" << endl;
	}
	system("pause");
	system("cls");
}

void WorkerManager::Exitsystem() {
	cout << "欢迎下次使用" << endl;
	system("pause");
	exit(0);
}