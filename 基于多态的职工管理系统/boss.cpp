#include "boss.h"

Boss::Boss(int id, string name, int dId) {
	this->m_id = id;
	this->m_Name = name;
	this->m_DeptId = dId;
}

void Boss::showInfo() {
	cout << "职工编号：" << this->m_id
		<< "\t职工姓名：" << this->m_Name
		<< "\t岗位：" << this->getDeptName()
		<< "\t岗位职责：领导公司" << endl;
}

string Boss::getDeptName() {
	return string("老板");
}