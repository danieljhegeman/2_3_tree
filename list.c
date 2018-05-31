#include <Python.h>

char * Cfib(int n)
{
  char *letter = malloc(sizeof(char));
  *letter = 'f';
  return letter; 
}

static PyObject * bfList(long x, long y)
{
  PyObject *nodesPyList = PyList_New(2);
  int i;
  PyList_SET_ITEM(nodesPyList, 0, PyInt_FromLong(x));
  PyList_SET_ITEM(nodesPyList, 1, PyInt_FromLong(y));
  return nodesPyList;
}

static PyObject * makeList(PyObject *self, PyObject *args)
{
  int length = 5;
  PyObject *list;
  int i;
  for (i = 0; i < length; ++i) {
    PyList_SET_ITEM(list, i, bfList((long) i + 3, (long) i * 2));
  }
  return list;
}

int other(int n, char *ptr)
{
  fprintf(stderr, "%d Char is: %c\n", n, *ptr);
  return 100;
}

static PyObject * fib(PyObject *self, PyObject *args)
{
  fprintf(stderr, "here\n");
  int n;
  if (!PyArg_ParseTuple(args, "i", &n)) {
    return NULL;
  } else {
    fprintf(stderr, "%d\n", n);
    return Py_BuildValue("k", Cfib(n));
  }
}

static PyObject * myOther(PyObject *self, PyObject *args)
{
  int n;
  char *p;
  if (!PyArg_ParseTuple(args, "ik", &n, &p)) {
    return NULL;
  } else {
    fprintf(stderr, "ptr val is %p\n", p);
    fprintf(stderr, "ptr points to %c\n", *p); 
    return Py_BuildValue("i", other(n, p));
  }
}

static PyObject * version(PyObject *self)
{
  return Py_BuildValue("s", "Version 1.0");
}

static PyMethodDef myMethods[] = {
  {"fib", fib, METH_VARARGS, "Calculates the fibonacci numbers."},
  {"myOther", myOther, METH_VARARGS, "Gives pointer."},
  {"version", (PyCFunction) version, METH_NOARGS, "returns the version."},
  {NULL, NULL, 0, NULL}
};

static struct PyModuleDef myModule = {
  PyModuleDef_HEAD_INIT,
  "myFibModule",
  "Fibonacci Module",
  -1, // global state
  myMethods
};

PyMODINIT_FUNC PyInit_myFibModule(void)
{
  return PyModule_Create(&myModule);
}
