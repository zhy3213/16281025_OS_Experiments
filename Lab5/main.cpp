//
//  main.cpp
//  FileSystem
//
//  Created by 张昊洋 on 2019/6/6.
//  Copyright © 2019 张昊洋. All rights reserved.
//

#include <iostream>
#include <string.h>
#include <iomanip>
#include <stdio.h>
#define CMAX  100
#define BMAX  100
#define HMAX  10

using namespace std;

typedef struct block
{
    char data[512];
    char flag;
    block* next;
}Block;


typedef struct
{
    char* name;
    Block* content;
}File;

typedef struct
{
    int index;
    int pos;
    Block* data;
}OpenedFile;

Block ldisk[CMAX][HMAX][BMAX];


char buff[100];

int read_block(int i, char* p)
{
    for (int j = 0; j < i; j++)
    {
        p[j] = buff[j];
    }
    return 1;
}
int write_block(int i, char* p)
{
    for (int j = 0; j < i; j++)
    {
        buff[j] = p[j];
    }
    return 1;
}
int create(char* filename)
{
    string ilegal[] = { "/","\\", ":","<",">","|","*","&" };
    File newfile;
    newfile.name = filename;

    for (int i = 0; i < 8; i++)
    {
        if (strchr(filename, ilegal[i][0]))
        {
            cout << "命名包含非法字符" << endl;
            break;
        }
    }
    int temp = 1;
    int pos = 0;
    for (int i = 0; i < BMAX; i++)
    {
        if (ldisk[0][0][i].flag == '1')
        {
            if (strcmp(ldisk[0][0][i].data,filename)==0)
            {
                temp = 0;
                break;
            }
        }
        else {
            pos = i;
        }
    }
    if (temp == 0)
    {
        cout << "已经有同名文件" << endl;
    }
    else {
        ldisk[0][0][pos].flag = '1';
        strcat(ldisk[0][0][pos].data, filename);
        for (int m = 1; m < CMAX; m++)
        {
            for (int n = 1; n < HMAX; n++)
            {
                for (int p =0 ; p < BMAX; p++)
                {
                    if (ldisk[m][n][p].flag != '1')
                    {
                        ldisk[m][n][p].flag = '1';
                        ldisk[0][0][pos].next = &ldisk[m][n][p];
                        break;
                    }
                }
            }
        }
        cout <<endl<< "创建成功" << endl;
    }
    return 1;
}

int destroy(char* filename)// 删除指定文件。
{
    int temp = 1;
    int pos = 0;
    // 找到文件
    for (int i = 0; i < BMAX; i++)
    {
        if (ldisk[0][0][i].flag == '1')
        {
            if (strcmp(ldisk[0][0][i].data, filename)== 0)
            {
                pos = i;
                break;
            }
        }
    }
    if (pos == 0)
    {
        cout << "error 未找到文件，无法删除" << endl;
    }
    else {
        ldisk[0][0][pos].flag = '0';
        ldisk[0][0][pos].data[0] = '\0';
        ldisk[0][0][pos].next = NULL;
        cout << "删除成功" << endl;
    }
    return 0;
}

OpenedFile items[CMAX];
int open(char* filename)
{
    int pos = 0;
    for (int i = 0; i < BMAX; i++)
    {
        if (ldisk[0][0][i].flag == '1')
        {
            if (strcmp(ldisk[0][0][i].data, filename)==0)
            {
                pos = i;
                break;
            }
        }
    }
    if (pos == 0)
    {
        cout << "未能找到文件" << endl;
    }
    else {
        cout <<endl<< "打开成功" << endl;
        
        for (int j = 0; j < CMAX; j++)
        {
            if (items[j].index == 0)
            {
                items[j].index = j;
                items[j].pos = 0;
                items[j].data = ldisk[0][0][pos].next;
                cout << "index: " << items[j].index << endl;
                return items[j].index;
            }
        }
    }
    return 0;
}

int close(int index)// 关闭制定文件。
{
    for (int j = 0; j < CMAX; j++)
    {
        if (items[j].index == index)
        {
            items[j].index = 0;
            items[j].pos = 0;
            items[j].data = NULL;
        }
        
    }
    cout <<endl<< "关闭成功" << endl;
    return 0;
}
int rename(char *old, char *new1)
{
    int pos = 0;
    // 找到文件
    for (int i = 0; i < BMAX; i++)
    {
        if (ldisk[0][0][i].flag == '1')
        {
            if (strcmp(ldisk[0][0][i].data, old)==0)
            {
                pos = i;
                break;
            }
        }
    }
    if (pos == 0)
    {
        cout << "未找到文件" << endl;
    }
    else {
        strcat(ldisk[0][0][pos].data, new1);
        cout <<endl<< "文件改名成功" << endl;
    }
    return 0;
}
int read(int index, int count)
{
    string mem_area("a",100);
    for (int j = 0; j < CMAX; j++)
    {
        if (items[j].index == index)
        {
            for (int i = 0; i < count; i++)
            {
                mem_area[i] = items[j].data->data[i + items[j].pos];
                cout << mem_area[i];
            }
            cout <<endl<< "读出完毕" << endl;
            return 0;
        }
    }
    cout << "找不到文件,请重试" << endl;
    return 0;
}
int write(int index, string mem_area, int count)
{
    for (int j = 0; j < CMAX; j++)
    {
        if (items[j].index == index)
        {
            Block* p = items[j].data;
            for (int i = 0; i < count; i++)
            {
                if (i+1 % 512 == 0)
                {
                    p->next = NULL;
                    p = p->next;
                }
                p->data[items[j].pos] = mem_area[i];
                items[j].pos++;
            }
            cout << "写入成功" << endl;
            return 0;
        }
    }
    cout << "找不到文件,请重试" << endl;
    return 0;
}
int lseek(int index, int pos)
{
    for (int j = 0; j < CMAX; j++)
    {
        if (items[j].index == index)
        {
            items[j].pos = pos;
            cout << "指针重置成功" << endl;
            return 0;
        }
    }
    cout << "找不到文件,请重试" << endl;
    return 0;
}

int dir() {
    for (int i = 0; i < BMAX; i++)
    {
        if (ldisk[0][0][i].flag == '1')
        {
            cout << ldisk[0][0][i].data << endl;
        }
    }
    return 0;
}
int show_path()
{
    cout << "~/";
    return 1;
}

void hint() {
    cout << "命令：" << endl;
    cout << "1.create <文件名>" << endl;
    cout << "2.destory <文件名>" << endl;
    cout << "3.rename <文件名> <新文件名>" << endl;
    cout << "4.open <文件名>" << endl;
    cout << "5.close <文件名>" << endl;
    cout << "6.write <index> <string>" << endl;
    cout << "7.read <index>" << endl;
    cout << "8.lseek <index> <newpos>" << endl;
    cout << "9.ls" << endl;
    cout << "10.exit" << endl;
}


int main() {
    while (1) {
        hint();
        while (1) {
            cout << endl;
            string choice;
            char name1[20],name2[20];
            int index, memarea, count;
            show_path();
            cin >> choice;
            if (choice == "create")
            {
                cin >> name1;
                create(name1);
            }
            else if (choice == "destory")
            {
                cin >> name1;
                destroy(name1);
            }
            else if (choice == "open")
            {
                cin >> name1;
                open(name1);
            }
            else if (choice == "close")
            {
                cin >> index;
                close(index);
            }
            else if (choice == "rename")
            {
                cin >> name1 >> name2;
                rename(name1,name2);
            }
            else if (choice == "write")
            {
                cin >> index;
                cin >> choice;
                count = choice.size();
                write(index, choice, count);
                
            }
            else if (choice == "read")
            {
                cin >> index;
                cin >> count;
                read(index, count);
            }
            else if (choice == "lseek")
            {
                cin >> index;
                cin >> memarea;
                lseek(index,memarea);
            }
            else if (choice == "ls")
            {
                dir();
            }
            else if (choice == "exit") {
                return 0;
            }
        }
    }
    return 0;
}
