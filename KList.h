//Василевская Яна, 2 группа
//линейный строго-упорядоченный однонаправленный список


#pragma once
#include<assert.h>
#include<iostream>
#include<cstring>

using namespace std;



//параметр шаблона - убывание или возрастание
typedef enum                   //decrease-убывание, increase-возрастание
{
	decr = 0,
	incr = 1,
} order_type;




template <order_type order, class T>
class Visitor;

template <order_type order, class T>
class MinVisitor;

template <order_type order, class T>
class MaxVisitor;




///////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////list//////////////////////////////////////////////////////
template <order_type order, class T>
class list
{
protected:

	struct element             //элементы списка (состоят из данных и указателя на след. элемент)
	{
		T data;
		element* ptr_next;

		element();
		element(T data);
		element(T data, element* ptr_next);
		element(const element& E);
		~element();
	};

	element* ptr_head;         //указатель на начальный элемент
	int size_data;             //количество элементов


public:

	explicit list();                                            //конструктор по умолчанию
	list(T* mas, int n);                                        //конструктор с параметрами (массив данных)
	list(const list<order, T>& L);                              //конструктор копирования
	~list();                                                    //деструктор

	int size();                                                 //возвращает размер
	void clear();                                               //очистка списка
	const list<order, T>& operator=(const list<order, T>& L);   //оператор присваивания
	T& operator[](const int index) const;                       //оператор []
	void add(T data);                                           //добавление элемента
	void remove(T data);                                        //удаление элемента


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////iterator//////////////////////////////////////////////////////////////////////////////
	class iterator
	{
	protected:
		element* ptr;

	public:

		iterator(element* ptr = nullptr) : ptr(ptr) {}    //конструктор по умолчанию + с параметром
		iterator(const iterator& I) : ptr(I.ptr) {}       //конструктор копирования
		iterator operator++(int)                          //++ (постфиксный)
		{
			iterator i = *this;
			ptr = ptr->ptr_next;
			return i;
		}
		iterator operator++()                             //++ (префиксный)
		{
			ptr = ptr->ptr_next;
			return *this;
		}
		bool operator==(const iterator& I) const          //==
		{
			return (ptr == I.ptr);
		}
		bool operator!=(const iterator& I) const          //!=
		{
			return (ptr != I.ptr);
		}
		T& operator*() const                              //* (разыменование)
		{
			assert(ptr != nullptr);
			return ptr->data;
		}
	};

	iterator begin()                                      //begin
	{
		return iterator(ptr_head);
	}
	iterator end()                                        //end
	{
		element* current = ptr_head;
		while (current != nullptr)
			current = current->ptr_next;
		return iterator(current);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////методы с visitor////////////////////////////////////////////////////////
	T accept(Visitor<order, T>& V)        //accept
	{
		return V.visit_list(*this);
	}
	T min()                               //min()
	{
		MinVisitor<order, T> minvis;
		T mv = accept(minvis);
		return mv;
	}
	T max()                               //max()
	{
		MaxVisitor<order, T> maxvis;
		T mv = accept(maxvis);
		return mv;
	}

	/////////////////////////////////////////////методы с iterator//////////////////////////////////////////////////////
	T imin()                              //imin
	{
		T value = *(begin());
		for (list<order, T>::iterator it = begin(); it != end(); ++it)
		{
			if (*it < value)
				value = *it;
		}
		return value;
	}
	T imax()                               //imax
	{
		T value = *(begin());
		for (list<order, T>::iterator it = begin(); it != end(); ++it)
		{
			if (*it > value)
				value = *it;
		}
		return value;
	}
};





////////////////////////////////////////////Visitor/////////////////////////////////////////////////////
template <order_type order, class T>
class Visitor
{
public:
	virtual T visit_list(list<order, T>& L) = 0;
	virtual ~Visitor() {}

protected:
	Visitor() {}
};

/////////////////////////////////////Visitor для min()////////////////////////////////////////
template <order_type order, class T>
class MinVisitor : public Visitor<order, T>
{
public:
	T value;
	T visit_list(list<order, T>& L)
	{
		value = L[0];
		for (int i = 1; i < L.size(); ++i)
			if (L[i] < value)
				value = L[i];
		return value;
	}
	MinVisitor() {}
	~MinVisitor() {}
};

template<>
MinVisitor<incr, char*>::~MinVisitor()
{
	if (value)
		delete[]value;
}

template<>
MinVisitor<decr, char*>::~MinVisitor()
{
	if (value)
		delete[]value;
}

template<>
char* MinVisitor<incr, char*>::visit_list(list<incr, char*>& L)
{
	if (value)
		delete[]value;
	value = new char[strlen(L[0]) + 1];
	strcpy_s(value, strlen(L[0]) + 1, L[0]);
	for (int i = 1; i < L.size(); ++i)
		if (strcmp(L[i], value) < 0)
		{
			delete[]value;
			value = new char[strlen(L[i]) + 1];
			strcpy_s(value, strlen(L[i]) + 1, L[i]);
		}
	return value;
}



template<>
char* MinVisitor<decr, char*>::visit_list(list<decr, char*>& L)
{
	if (value)
		delete[]value;
	value = new char[strlen(L[0]) + 1];
	strcpy_s(value, strlen(L[0]) + 1, L[0]);
	for (int i = 1; i < L.size(); ++i)
		if (strcmp(L[i], value) < 0)
		{
			delete[]value;
			value = new char[strlen(L[i]) + 1];
			strcpy_s(value, strlen(L[i]) + 1, L[i]);
		}
	return value;
}

////////////////////////////////////////Visitor для max()///////////////////////////////////////////////
template <order_type order, class T>
class MaxVisitor : public Visitor<order, T>
{
public:
	T value;
	T visit_list(list<order, T>& L)
	{
		value = L[0];
		for (int i = 1; i < L.size(); ++i)
			if (L[i] > value)
				value = L[i];
		return value;
	}
	~MaxVisitor() {}
};

template<>
MaxVisitor<incr, char*>::~MaxVisitor()
{
	if (value)
		delete[]value;
}

template<>
MaxVisitor<decr, char*>::~MaxVisitor()
{
	if (value)
		delete[]value;
}

template<>
char* MaxVisitor<incr, char*>::visit_list(list<incr, char*>& L)
{
	if (value)
		delete[]value;
	value = new char[strlen(L[0]) + 1];
	strcpy_s(value, strlen(L[0]) + 1, L[0]);
	for (int i = 1; i < L.size(); ++i)
		if (strcmp(L[i], value) > 0)
		{
			delete[]value;
			value = new char[strlen(L[i]) + 1];
			strcpy_s(value, strlen(L[i]) + 1, L[i]);
		}
	return value;
}

template<>
char* MaxVisitor<decr, char*>::visit_list(list<decr, char*>& L)
{
	if (value)
		delete[]value;
	value = new char[strlen(L[0]) + 1];
	strcpy_s(value, strlen(L[0]) + 1, L[0]);
	for (int i = 1; i < L.size(); ++i)
		if (strcmp(L[i], value) > 0)
		{
			delete[]value;
			value = new char[strlen(L[i]) + 1];
			strcpy_s(value, strlen(L[i]) + 1, L[i]);
		}
	return value;
}



template<>
char* list<incr, char*>::min()
{
	MinVisitor<incr, char*> minvis;
	char* mv = new char[strlen(accept(minvis))+1];
	strcpy_s(mv, strlen(accept(minvis)) + 1, accept(minvis));
	return mv;
}

template<>
char* list<decr, char*>::min()
{
	MinVisitor<decr, char*> minvis;
	char* mv = new char[strlen(accept(minvis)) + 1];
	strcpy_s(mv, strlen(accept(minvis)) + 1, accept(minvis));
	return mv;
}

template<>
char* list<incr, char*>::max()
{
	MaxVisitor<incr, char*> maxvis;
	char* mv = new char[strlen(accept(maxvis)) + 1];
	strcpy_s(mv, strlen(accept(maxvis)) + 1, accept(maxvis));
	return mv;
}

template<>
char* list<decr, char*>::max()
{
	MaxVisitor<decr, char*> maxvis;
	char* mv = new char[strlen(accept(maxvis)) + 1];
	strcpy_s(mv, strlen(accept(maxvis)) + 1, accept(maxvis));
	return mv;
}


/////////////////////////////////////////////////////imin для char*/////////////////////////////////////////
template<>
char* list<incr, char*>::imin()
{
	char* value = new char[strlen(*(begin())) + 1];
	strcpy_s(value, strlen(*(begin())) + 1, *(begin()));
	for (list<incr, char*>::iterator it = begin(); it != end(); ++it)
	{
		if (strcmp(*it, value) < 0)
		{
			delete[]value;
			value = new char[strlen(*it) + 1];
			strcpy_s(value, strlen(*it) + 1, *it);
		}
	}
	return value;
}


template<>
char* list<decr, char*>::imin()
{
	char* value = new char[strlen(*(begin())) + 1];
	strcpy_s(value, strlen(*(begin())) + 1, *(begin()));
	for (list<decr, char*>::iterator it = begin(); it != end(); ++it)
	{
		if (strcmp(*it, value) < 0)
		{
			delete[]value;
			value = new char[strlen(*it) + 1];
			strcpy_s(value, strlen(*it) + 1, *it);
		}
	}
	return value;
}

////////////////////////////////////////////////////////imax для char*///////////////////////////////////////
template<>
char* list<incr, char*>::imax()
{
	char* value = new char[strlen(*(begin())) + 1];
	strcpy_s(value, strlen(*(begin())) + 1, *(begin()));
	for (list<incr, char*>::iterator it = begin(); it != end(); ++it)
	{
		if (strcmp(*it, value) > 0)
		{
			delete[]value;
			value = new char[strlen(*it) + 1];
			strcpy_s(value, strlen(*it) + 1, *it);
		}
	}
	return value;
}

template<>
char* list<decr, char*>::imax()
{
	char* value = new char[strlen(*(begin())) + 1];
	strcpy_s(value, strlen(*(begin())) + 1, *(begin()));
	for (list<decr, char*>::iterator it = begin(); it != end(); ++it)
	{
		if (strcmp(*it, value) > 0)
		{
			delete[]value;
			value = new char[strlen(*it) + 1];
			strcpy_s(value, strlen(*it) + 1, *it);
		}
	}
	return value;
}








//конструктор element по умолчанию
template <order_type order, class T>
list<order, T>::element::element() {}





//конструктор element с 1 параметром
template <order_type order, class T>
list<order, T>::element::element(T data) : data(data), ptr_next(nullptr) {}


template <>
list<incr, char*>::element::element(char* data)
{
	this->data = new char[strlen(data) + 1];
	strcpy_s(this->data, strlen(data) + 1, data);
}


template <>
list<decr, char*>::element::element(char* data)
{
	this->data = new char[strlen(data) + 1];
	strcpy_s(this->data, strlen(data) + 1, data);
}





//конструктор element с параметрами
template <order_type order, class T>
list<order, T>::element::element(T data, element* ptr_next) : data(data), ptr_next(ptr_next) {}


template <>
list<incr, char*>::element::element(char* data, element* ptr_next) : ptr_next(ptr_next)
{
	this->data = new char[strlen(data) + 1];
	strcpy_s(this->data, strlen(data) + 1, data);
}

template <>
list<decr, char*>::element::element(char* data, element* ptr_next) : ptr_next(ptr_next)
{
	this->data = new char[strlen(data) + 1];
	strcpy_s(this->data, strlen(data) + 1, data);
}





//конструктор element копирования
template <order_type order, class T>
list<order, T>::element::element(const element& E) : data(E.data), ptr_next(E.ptr_next) {}


template <>
list<incr, char*>::element::element(const element& E) : ptr_next(E.ptr_next)
{
	data = new char[strlen(E.data) + 1];
	strcpy_s(data, strlen(E.data) + 1, E.data);
}


template <>
list<decr, char*>::element::element(const element& E) : ptr_next(E.ptr_next)
{
	data = new char[strlen(E.data) + 1];
	strcpy_s(data, strlen(E.data) + 1, E.data);
}





//деструктор element
template <order_type order, class T>
list<order, T>::element::~element() {}


template <>
list<incr, char*>::element::~element()
{
	delete[] data;
}


template <>
list<decr, char*>::element::~element()
{
	delete[] data;
}





//конструктор по умолчанию
template <order_type order, class T>
list<order, T>::list() : size_data(0)
{
	ptr_head = nullptr;
}





//конструктор с параметрами (массив данных)
//не копирует списки с различным типами упорядочивания (incr и decr)
template <order_type order, class T>
list<order, T>::list(T* mas, int n) : size_data(n)
{
	assert(n >= 0/*размер массива меньше 0*/);
	if (n == 0)
	{
		ptr_head = nullptr;
		return;
	}
	for (int i = 0; i < size_data; ++i)            //убираем из массива повторяющиеся значения
		for (int j = i + 1; j < size_data; ++j)
			if (mas[i] == mas[j])
			{
				swap(mas[j], mas[size_data - 1]);
				--size_data;
				--j;
			}
	element* mas_el = new element[size_data];
	for (int i = 0; i < size_data; ++i)
		mas_el[i].data = mas[i];
	for (int i = 0; i < size_data; ++i)
		for (int j = 0; j < i; ++j)
		{
			if (order)
			{
				if (mas_el[i].data < mas_el[j].data)
					swap(mas_el[i].data, mas_el[j].data);
			}
			else
			{
				if (mas_el[i].data > mas_el[j].data)
					swap(mas_el[i].data, mas_el[j].data);
			}
		}
	for (int i = 0; i < size_data - 1; ++i)
		mas_el[i].ptr_next = &mas_el[i + 1];
	mas_el[size_data - 1].ptr_next = nullptr;
	ptr_head = &mas_el[0];
}


template <>
list<incr, char*>::list(char** mas, int n) : size_data(n)
{
	assert(n >= 0/*размер массива меньше 0*/);
	if (n == 0)
	{
		ptr_head = nullptr;
		return;
	}
	for (int i = 0; i < size_data; ++i)            //убираем из массива повторяющиеся значения
		for (int j = i + 1; j < size_data; ++j)
			if (!strcmp(mas[i], mas[j]))
			{
				char* temp = new char[strlen(mas[i]) + 1];
				strcpy_s(temp, strlen(mas[i]) + 1, mas[i]);
				mas[i] = new char[strlen(mas[j]) + 1];
				strcpy_s(mas[i], strlen(mas[j]) + 1, mas[j]);
				mas[j] = new char[strlen(temp) + 1];
				strcpy_s(mas[j], strlen(temp) + 1, temp);
				delete[] temp;
				--size_data;
				--j;
			}
	element* mas_el = new element[size_data];
	for (int i = 0; i < size_data; ++i)
	{
		mas_el[i].data = new char[strlen(mas[i]) + 1];
		strcpy_s(mas_el[i].data, strlen(mas[i]) + 1, mas[i]);
	}
	for (int i = 0; i < size_data; ++i)
		for (int j = 0; j < i; ++j)
			if (strcmp(mas_el[i].data, mas_el[j].data) < 0)
			{
				char* temp = new char[strlen(mas_el[i].data) + 1];
				strcpy_s(temp, strlen(mas_el[i].data) + 1, mas_el[i].data);
				delete[] mas_el[i].data;
				mas_el[i].data = new char[strlen(mas_el[j].data) + 1];
				strcpy_s(mas_el[i].data, strlen(mas_el[j].data) + 1, mas_el[j].data);
				delete[] mas_el[j].data;
				mas_el[j].data = new char[strlen(temp) + 1];
				strcpy_s(mas_el[j].data, strlen(temp) + 1, temp);
				delete[] temp;
			}
	for (int i = 0; i < size_data - 1; ++i)
		mas_el[i].ptr_next = &mas_el[i + 1];
	mas_el[size_data - 1].ptr_next = nullptr;
	ptr_head = &mas_el[0];
}


template <>
list<decr, char*>::list(char** mas, int n) : size_data(n)
{
	assert(n >= 0/*размер массива меньше 0*/);
	if (n == 0)
	{
		ptr_head = nullptr;
		return;
	}
	for (int i = 0; i < size_data; ++i)            //убираем из массива повторяющиеся значения
		for (int j = i + 1; j < size_data; ++j)
			if (!strcmp(mas[i], mas[j]))
			{
				char* temp = new char[strlen(mas[i]) + 1];
				strcpy_s(temp, strlen(mas[i]) + 1, mas[i]);
				mas[i] = new char[strlen(mas[j]) + 1];
				strcpy_s(mas[i], strlen(mas[j]) + 1, mas[j]);
				mas[j] = new char[strlen(temp) + 1];
				strcpy_s(mas[j], strlen(temp) + 1, temp);
				delete[] temp;
				--size_data;
				--j;
			}
	element* mas_el = new element[size_data];
	for (int i = 0; i < size_data; ++i)
	{
		mas_el[i].data = new char[strlen(mas[i]) + 1];
		strcpy_s(mas_el[i].data, strlen(mas[i]) + 1, mas[i]);
	}
	for (int i = 0; i < size_data; ++i)
		for (int j = 0; j < i; ++j)
			if (strcmp(mas_el[i].data, mas_el[j].data) > 0)
			{
				char* temp = new char[strlen(mas_el[i].data) + 1];
				strcpy_s(temp, strlen(mas_el[i].data) + 1, mas_el[i].data);
				delete[] mas_el[i].data;
				mas_el[i].data = new char[strlen(mas_el[j].data) + 1];
				strcpy_s(mas_el[i].data, strlen(mas_el[j].data) + 1, mas_el[j].data);
				delete[] mas_el[j].data;
				mas_el[j].data = new char[strlen(temp) + 1];
				strcpy_s(mas_el[j].data, strlen(temp) + 1, temp);
				delete[] temp;
			}
	for (int i = 0; i < size_data - 1; ++i)
		mas_el[i].ptr_next = &mas_el[i + 1];
	mas_el[size_data - 1].ptr_next = nullptr;
	ptr_head = &mas_el[0];
}





//конструктор копирования
template <order_type order, class T>
list<order, T>::list(const list<order, T>& L) : size_data(L.size_data)
{
	if (L.ptr_head == nullptr || L.size_data == 0)
	{
		ptr_head = nullptr;
		return;
	}
	ptr_head = new element(L.ptr_head->data);
	element* current = L.ptr_head->ptr_next;
	element* last = ptr_head;
	while (current != nullptr)
	{
		element* b = new element(current->data);
		last->ptr_next = b;
		last = b;
		current = current->ptr_next;
	}
	last->ptr_next = nullptr;
}





//деструктор
template <order_type order, class T>
list<order, T>::~list()
{
	clear();
}





//очистка списка
template <order_type order, class T>
void list<order, T>::clear()
{
	element* current = ptr_head;
	while (ptr_head != nullptr)
	{
		current = ptr_head->ptr_next;
		ptr_head->~element();
		ptr_head = current;
	}
	ptr_head = nullptr;
	size_data = 0;
}





//возврат размера
template <order_type order, class T>
int list<order, T>::size()
{
	return size_data;
}





//оператор присваивания
template<order_type order, class T>
const list<order, T>& list<order, T>::operator=(const list<order, T>& L)
{
	clear();
	if (L.ptr_head == nullptr || L.size_data == 0)
	{
		ptr_head = nullptr;
		return *this;
	}
	ptr_head = new element(L.ptr_head->data);
	element* current = L.ptr_head->ptr_next;
	element* last = ptr_head;
	while (current != nullptr)
	{
		element* b = new element(current->data);
		last->ptr_next = b;
		last = b;
		current = current->ptr_next;
	}
	last->ptr_next = nullptr;
	size_data = L.size_data;
	return *this;
}





//оператор []
template <order_type order, class T>
T& list<order, T>::operator[](const int index) const
{
	assert(index >= 0);
	assert(index < size_data);
	int counter = 0;
	element* current = ptr_head;
	while (current != nullptr)
	{
		if (counter == index)
			return current->data;
		current = current->ptr_next;
		counter++;
	}
	return ptr_head->data;
}





//добавление элемента по значению
template <order_type order, class T>
void list<order, T>::add(T data)
{
	if (ptr_head == nullptr)
	{
		ptr_head = new element(data);
		size_data = 1;
	}
	else
	{
		element* next = ptr_head;
		element* previous = nullptr;
		if (order)
		{
			if (next->data > data)
			{
				ptr_head = new element(data, ptr_head);
				++size_data;
				return;
			}
			while (next != nullptr && next->data < data)
			{
				previous = next;
				next = next->ptr_next;
			}
			if (next != nullptr && next->data == data)
				return;
			element* new_element = new element(data, previous->ptr_next);
			previous->ptr_next = new_element;
		}
		else
		{
			if (next->data < data)
			{
				ptr_head = new element(data, ptr_head);
				++size_data;
				return;
			}
			while (next != nullptr && next->data > data)
			{
				previous = next;
				next = next->ptr_next;
			}
			if (next != nullptr && next->data == data)
				return;
			element* new_element = new element(data, previous->ptr_next);
			previous->ptr_next = new_element;
		}
	}
	++size_data;
}


template <>
void list<incr, char*>::add(char* data)
{
	if (ptr_head == nullptr)
	{
		ptr_head = new element(data);
		size_data = 1;
	}
	else
	{
		element* next = ptr_head;
		element* previous = nullptr;
		if (strcmp(next->data, data) > 0)
		{
			ptr_head = new element(data, ptr_head);
			++size_data;
			return;
		}
		while (next != nullptr && strcmp(next->data, data) < 0)
		{
			previous = next;
			next = next->ptr_next;
		}
		if (next != nullptr && strcmp(next->data, data) == 0)
			return;
		element* new_element = new element(data, previous->ptr_next);
		previous->ptr_next = new_element;
	}
	++size_data;
}


template <>
void list<decr, char*>::add(char* data)
{
	if (ptr_head == nullptr)
	{
		ptr_head = new element(data);
		size_data = 1;
	}
	else
	{
		element* next = ptr_head;
		element* previous = nullptr;
		if (strcmp(next->data, data) < 0)
		{
			ptr_head = new element(data, ptr_head);
			++size_data;
			return;
		}
		while (next != nullptr && strcmp(next->data, data) > 0)
		{
			previous = next;
			next = next->ptr_next;
		}
		if (next != nullptr && strcmp(next->data, data) == 0)
			return;
		element* new_element = new element(data, previous->ptr_next);
		previous->ptr_next = new_element;
	}
	++size_data;
}





//удаление элемента по значению
template <order_type order, class T>
void list<order, T>::remove(T data)
{
	assert(ptr_head/*нет элементов в списке*/);
	element* next = ptr_head;
	element* previous = nullptr;
	if (order)
	{
		while (next != nullptr && next->data < data)
		{
			previous = next;
			next = next->ptr_next;
		}
		if (next == nullptr) //нет элемента с таким значением
			return;
		assert(next->data == data/*нет элемента с таким значением*/);
		if (previous == nullptr)
		{
			ptr_head = next->ptr_next;
		}
		else
			previous->ptr_next = next->ptr_next;
		next->~element();
	}
	else
	{
		while (next != nullptr && next->data > data)
		{
			previous = next;
			next = next->ptr_next;
		}
		if (next == nullptr) //нет элемента с таким значением
			return;
		assert(next->data == next->data/*нет элемента с таким значением*/);
		if (previous == nullptr)
		{
			ptr_head = next->ptr_next;
		}
		else
			previous->ptr_next = next->ptr_next;
		next->~element();
	}
	--size_data;
}


template <>
void list<incr, char*>::remove(char* data)
{
	assert(ptr_head/*нет элементов в списке*/);
	element* next = ptr_head;
	element* previous = nullptr;
	while (next != nullptr && strcmp(next->data, data) < 0)
	{
		previous = next;
		next = next->ptr_next;
	}
	if (next == nullptr) //нет элемента с таким значением
		return;
	assert(!strcmp(next->data, data)/*нет элемента с таким значением*/);
	if (previous == nullptr)
	{
		ptr_head = next->ptr_next;
	}
	else
		previous->ptr_next = next->ptr_next;
	next->~element();
	--size_data;
}


template <>
void list<decr, char*>::remove(char* data)
{
	assert(ptr_head/*нет элементов в списке*/);
	element* next = ptr_head;
	element* previous = nullptr;
	while (next != nullptr && strcmp(next->data, data) > 0)
	{
		previous = next;
		next = next->ptr_next;
	}
	if (next == nullptr) //нет элемента с таким значением
		return;
	assert(!strcmp(next->data, data)/*нет элемента с таким значением*/);
	if (previous == nullptr)
	{
		ptr_head = next->ptr_next;
	}
	else
		previous->ptr_next = next->ptr_next;
	next->~element();
	--size_data;
}
