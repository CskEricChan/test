#include<vector>
#include<stack>
#include<cstdio>
#include<iostream>
#include<cstdlib>
#include<cstring>
using namespace std;

bool IsLeft(char ch)
{
	return ch == '(';
}

bool IsRight(char ch)
{
	return ch == ')';
}

bool IsNumber(char ch)
{
	return (ch >= '0' && ch<= '9')||ch =='.';
}

bool IsOperator(char ch)
{
	return ch=='+'||ch=='-'||ch=='*'||ch=='/';
}

int GetNumber(string &buf, string &str, int index, bool negative)
{
	string s;
	float n=0;
	int in = index;
	if(negative == true)
	{
		buf += str[in];
		in++;
	}

	while(1)
	{
		if(IsNumber(str[in]))
		{
			buf += str[in];
		}
		else
			break;
		in++;
	}

	if(buf.size()==0 )
	{
		cout<<"Expecting a number!"<<endl;
		exit(0);
	}

	return in;
}

int get_operator(string &str,vector<string> &vec, stack<string> &stk, int ind)
{
	int index = ind;
	if(IsOperator(str[index]))
	{
		if(stk.empty())
		{	
			string tmp;
			tmp+= str[index];
			stk.push(tmp);
		}
		else if(str[index] == '+' || str[index]=='-')
		{
			if(stk.top()=="(")
			{
				string tmp;
				tmp+= str[index];
				stk.push(tmp);

			}
			else
			{
				string tmp;
				tmp+= str[index];
				while(!stk.empty())
				{
					vec.push_back(stk.top());
					stk.pop();
				}
				stk.push(tmp);
			}
		}
		else if(str[index] == '*' || str[index]=='/')
		{
			if(stk.top()=="+"||stk.top()=="-"||stk.top()=="(")
			{
				string tmp;
				tmp+= str[index];
				stk.push(tmp);
			}
			else
			{
				string tmp;
				tmp += str[index];
				while(!stk.empty())
				{
					vec.push_back(stk.top());
					stk.pop();
				}
				stk.push(tmp);
			}
		}
		index++;
	}
	else if(index != str.size())
	{
		exit(0);
		cout<<"Expecting An Operator : "<< index <<endl;
	}
	return index;
}

void get_expression(string &str, vector<string> &vec, stack<string> &stk)
{
	float n = 0;
	bool negative = false;
	int index = 0;

	if(str[index] == '-')
		negative = true;

	while(index < str.size())
	{
		if(IsLeft(str[index]))
		{
			string tmp;
			tmp+= str[index];
			stk.push(tmp);
			index++;
			if(IsLeft(str[index]) || IsNumber(str[index]))
				continue;
			else if(str[index] == '-')
			{
				negative = true;
				continue;
			}
			else
			{
				cout<<"Expecting a left bracket or a number : "<<index<<endl;
				exit(0);
			}
		}
		else if(IsRight(str[index]))
		{
			bool flag = false;
			while(!stk.empty())
			{
				if(IsLeft(stk.top().at(0)))
				{
					flag = true;
					stk.pop();
					break;
				}
				vec.push_back(stk.top());
				stk.pop();
			}

			if(flag == false)
			{
				cout<<"Can't find the Left bracket : "<<index;
				exit(0);
			}
			else
			{
				index++;
				if(index >= str.size())
					break;
				else if(IsOperator(str[index]))
				{
					index = get_operator(str,vec, stk, index);	
					continue;
				}
				else if(IsRight(str[index]))
					continue;
				else
				{
					cout<<"Expecting a right bracket or a operator."<<endl;
					exit(0);
				}
			}
		}

		if(IsNumber(str[index]) || negative==true){
			string buf;
			index = GetNumber(buf, str, index, negative);
			vec.push_back(buf);
			negative = false;
			if(index >= str.size())
				break;
			else if(IsRight(str[index]))
				continue;
		}
		else
		{
			cout<<"Expecting A Number : "<< index <<endl;
			exit(0);
		}

		index = get_operator(str,vec, stk, index);
	}

	while(!stk.empty())
	{
		if(IsLeft(stk.top().at(0)))
		{
			cout<<"Can't find the Left bracket : "<<index;
			exit(0);
		}
		vec.push_back(stk.top());
		stk.pop();
	}
}

float calc(float a, float b, string op)
{
	float ret = 0;
	switch(op[0])
	{
		case '+':
			ret = a + b;
			break;
		case '-':
			ret = a - b;
			break;
		case '*':
			ret = a * b;
			break;
		case '/':
			ret = a / b;
			break;
		default:
			break;
	}
	return ret;
}

void calc_expression( vector<string> &vec, stack<string> &stk)
{
	float a, b, t;
	for(auto it  = vec.begin(); it!= vec.end();it++)
	{
		if(*it == "+" || *it == "-" || *it == "*" || *it == "/" )
		{
			char buf[100];
			b =atof( stk.top().c_str() );
			stk.pop();
			a =atof( stk.top().c_str() );
			stk.pop();
			t = calc(a,b,*it);
			sprintf(buf, "%f", t);
			stk.push(buf);
		}
		else
		{
			stk.push(*it);	
		}
	}
}


int main()
{
	vector<string> vec;
	stack<string> stk;

	string str;

	cout<< "Please input a formula : ";
	cin >> str;

	//geting backing expression
	get_expression(str, vec, stk);

	cout<<"Backing expression : "<<endl;
	for(vector<string>::iterator it = vec.begin(); it!=vec.end(); it++)
	{
		cout << *it << " ";
	}
	cout<<endl;

	//calculate backing expression
	calc_expression( vec, stk);

	cout<< "The result is : "<<stk.top()<<endl;

	return 0;
}
