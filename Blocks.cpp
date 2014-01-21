// popravka_1zad_B.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include "MyStack.h"
#include <fstream>
using namespace std;

class Block{
	char name[100];
	int id;
	int defValue;
	stack<Block*> childs;
	
	void copy(const Block& b)
	{
		strcpy_s(name,b.name);
		id=b.id;
		defValue=b.defValue;
		childs=b.childs;
	}

public:
	Block()
	{
		strcpy_s(name,"master");
		id=0;
		defValue=0;
	}

	Block(char _name[100], int _id, int _default)
	{
		strcpy_s(name,_name);
		id=_id;
		defValue=_default;
	}

	Block(const Block& b)
	{
		copy(b);
	}

	Block& operator=(const Block& b)
	{
		if(this!=&b)
		{
			copy(b);
		}

		return *this;
	}

	void addSubBlock(Block* x)
	{
		childs.push(x);
	}


	Block* find(char* name)
	{
		Block* t = findBlock(name, this);
		return t;
	}

	Block* findBlock(char* name, Block* cur )
	{
		if( strcmp(name, cur->name) == 0)
		{
			return cur;
		}
		Block* c;
		while(!cur->childs.empty())
		{
			cur->childs.top(c);
			if(strcmp(c->name,name)==0)
			{
				return c;			
			}
			else 
				c = findBlock(name, c);

			if( strcmp(c->name,name)==0)
			{
				return c;				
			}
			else
				cur->childs.pop(c);

		}
	}

	int getBlockValue(char* name)
	{
		Block* b;
		b=find(name);
		return getValueIter(b);
	}
	
	int getValueIter(Block* cur)
	{
		if(cur->childs.empty())
			return cur->defValue;

		else
		{
			int value=0;
			while(!cur->childs.empty())
			{
				Block* child;
				cur->childs.pop(child);
				value+=getValueIter(child);
			}
			
			return value;
		}

	}

	char* getName() const
	{
		char* n=new char[strlen(name)+1];
		int i;
		for( i=0;name[i]!='\0';i++)
			n[i]=name[i];
		n[i] = '\0';
		return n;

	}

};


void writeValues(Block& x, int i)
{
	ofstream file("Block.bin", ios::out|ios::binary);
	char name[100];
	char* n;
	int v;
	file.seekp(i * (sizeof(name)+sizeof(v)),ios::beg);
	n=x.getName();
	v=x.getBlockValue("master");
	file.write((char*)&n, sizeof(n));
	file.write((char*)&v, sizeof(v));

	file.close();

	int p;
	char* gn;
	ifstream file2("Block.bin", ios::in);
	file2.seekg(i * (sizeof(name)+sizeof(p)),ios::beg);
	file2.read((char*)&gn, sizeof(gn));
	file2.read((char*)&p, sizeof(p));
	cout<<gn<<" "<<p;

	file2.close();

}


std::istream& operator>>(std::istream& file, Block& bl)
{
	char s;
	char curName[100];
	int id,def;	
	file>>s>>curName>>id>>def;

	Block temp(curName,id,def);
	bl=temp;

	Block* cur=&bl;
	while(file>>s)
	{
		if(s=='{')
		{
			file>>curName>>id>>def;
			Block* newBlock=new Block(curName,id,def);
			cur->addSubBlock(newBlock);
		}

		if(file>>s && s=='{')
		{
			cur=cur->find(curName);
			file.seekg(-1,ios::cur);
		}

	}

	return file;
}


int main()
{	
	Block a("master",1,1);
	Block b("child1",5,6);
	Block c("child2",2,10);
	a.addSubBlock(&b);
	a.addSubBlock(&c);
	
	Block d("subblock1",3,5);
	Block* temp;
	temp=a.find("child2");
	temp->addSubBlock(&d);
	
	writeValues(a,5);

	Block get;
	ifstream file("Block.txt",ios::in);
	file>>get;
	char* name;
	name=get.getName();
	cout<<name<<" "<<get.getBlockValue(name);
	return 0;
}
