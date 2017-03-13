#include<iostream>
#include<vector>
using namespace std;
void fillVector(vector<int> &vector_int)
{
	vector_int.push_back(1);
	vector_int.push_back(3);
	vector_int.push_back(4);
	vector_int.push_back(6);
	vector_int.push_back(8);
	vector_int.push_back(10);
}
int searchKey(vector<int> &vector_int,const int key)
{
	for (int i = 0; i < vector_int.size(); ++i)
	{
		if (vector_int[i] == key)
		{
			return vector_int[i];
		}
	}
	return -1;
}
void print(const int key)
{
	cout << key;
}
int main()
{
	const int key = 6;
	vector<int> vector_int;
	fillVector(vector_int);
	const int result = searchKey(vector_int, key);
	print(result);

}


