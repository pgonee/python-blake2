#include <Python.h>
#include "blake2.h"


static PyObject *Blake2HashSizeError;

char table[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
char stable[16] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\n'};

static PyObject * blake2b_func(PyObject *self, PyObject *args,  PyObject *keywds)
{

    char *data;
    int data_length;
    int key_length = 0;
    long hashSize = BLAKE2B_OUTBYTES;
    char *key = "";
    int rawOutput = 1 ;
    static char *kwlist[] = {"data","hashSize", "key", "rawOutput", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "z#|is#h", kwlist, &data, &data_length, &hashSize, &key, &key_length, &rawOutput)){
	Py_INCREF(Py_None);
	return Py_None;
    }

    if( NULL == data){
	Py_INCREF(Py_None);
	return Py_None;
    }

    char outputHash[ hashSize ];
    int result ;

    if ( ! ((  hashSize  > 0 ) && ( hashSize <= 64 ))){
        PyErr_SetString(Blake2HashSizeError, "hashSize error");
	Py_INCREF(Py_None);
	return Py_None;
    }	


    result = blake2b( outputHash, data, key, hashSize, data_length, key_length );

    if ( rawOutput == 1 ){
	char HexOutputHash[ hashSize * 2 + 1 ];
	for(int i=0; i<hashSize;i++){
		int a =  ( outputHash[i] >> 4 ) &0x0F;
		int b = outputHash[i]  		&0x0F;
		HexOutputHash[ i * 2 ] = table[a] ;
		HexOutputHash[ i * 2 + 1 ] = table[b] ;
	}
    	return Py_BuildValue("s#", HexOutputHash , hashSize * 2 );
    }
    return Py_BuildValue("s#", outputHash,hashSize);
}

static PyObject * blake2s_func(PyObject *self, PyObject *args,  PyObject *keywds)
{

    char *data;
    int data_length;
    int key_length = 0;
    long hashSize = BLAKE2S_OUTBYTES;
    char *key = "";
    int rawOutput = 1 ;
    static char *kwlist[] = {"data","hashSize", "key", "rawOutput", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "z#|is#h", kwlist, &data, &data_length, &hashSize, &key, &key_length, &rawOutput)){
	Py_INCREF(Py_None);
	return Py_None;
    }

    if( NULL == data){
	Py_INCREF(Py_None);
	return Py_None;
    }

    char outputHash[ hashSize ];
    int result ;

    if ( ! ((  hashSize  > 0 ) && ( hashSize <= 32 ))){
        PyErr_SetString(Blake2HashSizeError, "hashSize error");
	Py_INCREF(Py_None);
	return Py_None;
    }	


    result = blake2s( outputHash, data, key, hashSize, data_length, key_length );

    if ( rawOutput == 1 ){
	char HexOutputHash[ hashSize * 2 + 1 ];
	for(int i=0; i<hashSize;i++){
		int a =  ( outputHash[i] >> 4 ) &0x0F;
		int b = outputHash[i]  		&0x0F;
		HexOutputHash[ i * 2 ] = table[a] ;
		HexOutputHash[ i * 2 + 1 ] = table[b] ;
	}
    	return Py_BuildValue("s#", HexOutputHash , hashSize * 2 );
    }
    return Py_BuildValue("s#", outputHash,hashSize);
}



static PyMethodDef Blake2Methods[] =
{
    {"blake2",  blake2b_func, METH_VARARGS | METH_KEYWORDS , "return blake2"},
    {"blake2b",  blake2b_func, METH_VARARGS | METH_KEYWORDS , "return blake2"},
    {"blake2s",  blake2s_func, METH_VARARGS | METH_KEYWORDS , "return blake2"},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
initblake2(void)
{
     PyObject *b2;
     b2 = Py_InitModule("blake2", Blake2Methods);
     if ( b2 == NULL )
         return;

    Blake2HashSizeError = PyErr_NewException("blake2.Blake2HashSizeError", NULL, NULL);
    Py_INCREF(Blake2HashSizeError);
    PyModule_AddObject(b2, "Blake2HashSizeError", Blake2HashSizeError );
}
