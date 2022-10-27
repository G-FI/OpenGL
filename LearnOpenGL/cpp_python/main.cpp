#include<Python.h>
#include<stdio.h>

/*
PyObject *pName, *pModule, *pFunc, *pArgs, *pValue;
pName = PyUnicode_FromString((char*)"script");
pModule = PyImport_Import(pName);
pFunc = PyObject_GetAttrString(pModule, (char*)"test");
pArgs = PyTuple_Pack(1, PyUnicode_FromString((char*)"Greg"));
pValue = PyObject_CallObject(pFunc, pArgs);
auto result = _PyUnicode_AsString(pValue);
std::cout << result << std::endl;
*/

/*
//Run a simple file
FILE* PScriptFile = fopen("test.py", "r");
if(PScriptFile){
PyRun_SimpleFile(PScriptFile, "test.py");
fclose(PScriptFile);
}
*/
int run_func_mytest() {
	PyObject* pModuleName, * pModule, * pFunc, * pArgs = NULL, * pValue;
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('E:/code/OpenGL/LearnOpenGL/cpp_python/')");

	pModule = PyImport_ImportModule("mytest");

	
	pFunc = PyObject_GetAttrString(pModule, "func");

	//构造参数
	PyObject* pArgT = PyTuple_New(1);
	PyObject* pArgD = PyDict_New();
	float arg[] = { 1, 2, 3, 4, 5 };
	PyDict_SetItemString(pArgD, "data", Py_BuildValue("[i, i, i, i, i]", arg[0], arg[1], arg[2], arg[3], arg[4]));
	PyDict_SetItemString(pArgD, "index", Py_BuildValue("[i]", 10));
	//参数总体放入一个tuple中
	PyTuple_SetItem(pArgT, 0, pArgD);

	pValue = PyObject_CallObject(pFunc, pArgT);

	char* result;
	PyArg_Parse(pValue, "s", &result);
	printf("%s\n",result);
	return 0;
}
int run_func_hw2() {
	PyObject* pModuleName, * pModule, * pFunc, * pArgs = NULL, * pValue;
	PyRun_SimpleString("import sys");

	//E:\code\GAMES102\homeworks\hw2\hw2.py
	PyRun_SimpleString("sys.path.append('E:\\\\code\\\\GAMES102\\\\homeworks\\\\hw2')");
	PyRun_SimpleString("print(sys.path)");
	PyRun_SimpleString("import os");
	PyRun_SimpleString("print(os.listdir())");
	PyRun_SimpleString("import numpy");

	pModule = PyImport_ImportModule("hw2");


	pFunc = PyObject_GetAttrString(pModule, "mytest");

	//构造参数
	//PyObject* pArgT = PyTuple_New(1);
	//PyObject* pArgD = PyDict_New();

	//float arg[] = { 1, 2, 3, 4, 5 };
	//PyDict_SetItemString(pArgD, "data", Py_BuildValue("[i, i, i, i, i]", arg[0], arg[1], arg[2], arg[3], arg[4]));
	//PyDict_SetItemString(pArgD, "index", Py_BuildValue("[i]", 10));
	//
	////参数总体放入一个tuple中
	//PyTuple_SetItem(pArgT, 0, pArgD);

	pValue = PyObject_CallObject(pFunc, NULL);

	int result;
	PyArg_Parse(pValue, "i", &result);
	printf("%d\n", result);
	return 0;
}

int main(){
	Py_SetPythonHome(L"E:\\Anaconda3\\envs\\rl");
	//Py_SetPythonHome(L"E:\\Python");
	Py_Initialize();
	run_func_mytest();
	//run_func_hw2();
	//为什么虚拟环境中，导包倒不了

	/*PyRun_SimpleString("import sys");

	PyRun_SimpleString("sys.path.append('E:/code/GAMES102/homeworks/hw2/')");
	PyObject* pModule = PyImport_ImportModule("hw2");*/
	
	Py_Finalize();
}
