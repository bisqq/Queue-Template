#pragma once

#include <stdexcept>
#include <iostream>


// Macro
// To print easier instead of writing cout over again
#define LOG(x) std::cout << x;

// Macro
// I realized that in a lot of functions I am doing the same thing so I decided to simplify it
// has a x param to test for nullptr and has an optional param for returning
#define isPointerNull(x, ...) if(x == nullptr) return __VA_ARGS__;



template<class T>
class NodeT
{
public:

	// vars
	T data;
	NodeT<T>* next;

	// constructors to init class variables
	NodeT(NodeT<T>* tPtr, T tVal)
		:next(tPtr), data(tVal)
	{};

	NodeT(T val)
		:data(val), next(nullptr)
	{};

};

template<class T>
class QueueT
{
public:

	// defualt constructor
	// creates an empty queue
	QueueT();

	// copy constructor
	// creates a deep copy
	QueueT(const QueueT<T>& copy);

	// destructor
	// deallocates dynamic memory allocated by the queue
	~QueueT();

	// operator=
	// deeps copys and deallocates dynamic memory
	QueueT<T>& operator=(const QueueT<T>& operatorParam);

	// enqueue
	// inserts its template type parameter at the back of the queue
	void enqueue(T n);

	// dequeue
	// removes and returns the front of the queue
	T dequeue();

	// print
	void print() const;

	// empty
	// tests if the queue is empty
	bool empty() const;

	// size
	// returns queue size
	int size() const;

	// concatenate
	// concatenates two queues
	void concatenate(QueueT<T>& concat, int n);

	// merge
	// alternating merge  of two queues
	QueueT<T> const merge(const QueueT<T>& mergeList);

	// Getfront
	// returns front pointer 
	NodeT<T>* getFront();

private:

	// helpers
	// deep copy helper
	void copyQueueHelper(const QueueT<T>& copyHelper);
	// remove from queue helper
	void removeAll();

	// front and back pointer of the queue, and currentSize to keep track of the size of the queue
	NodeT<T>* frontPtr;
	NodeT<T>* backPtr;
	int currentSize;
};

template<class T>
QueueT<T>::QueueT()
{
	// sets front and back to null and queue size to 0
	frontPtr = backPtr = nullptr;
	currentSize = 0;
}

template<class T>
QueueT<T>::QueueT(const QueueT<T>& copy)
{
	// calls the helper method to copy
	copyQueueHelper(copy);
}

template<class T>
QueueT<T>::~QueueT()
{
	// calls the helper method to remove and deallocate
	removeAll();
}

template<class T>
QueueT<T>& QueueT<T>::operator=(const QueueT<T>& x)
{
	if (this != &x)
	{
		//calls helper methods
		removeAll();
		copyQueueHelper(x);
	}

	return *this;
}

template<class T>
void QueueT<T>::copyQueueHelper(const QueueT<T>& copy)
{

	// checks if the param front pointer is null
	isPointerNull(copy.frontPtr);

	// sets front and back pointer to nullptr and queue size to 0
	frontPtr = backPtr = nullptr;

	//LOG(std::endl << "Currentsize before: " << currentSize << std::endl);
	//LOG(std::endl << "Copy Param Currentsize before: " << copy.currentSize << std::endl);
	currentSize = 0;

	// copies the data
	currentSize = copy.currentSize;
	backPtr = copy.backPtr;

	//LOG(std::endl << "Currentsize after: " << currentSize << std::endl);
	//LOG(std::endl << "Copy Param Currentsize after: " << copy.currentSize << std::endl);

	// iterator node to go through the queue
	NodeT<T>* curr = copy.frontPtr;

	// loop through the queue and use the enqueue to create a copy
	for (int i = 0; i < copy.currentSize; i++)
	{
		enqueue(curr->data);
		curr = curr->next;
	}

}

template<class T>
void QueueT<T>::enqueue(T n)
{
	// create a new node
	NodeT<T>* newNode = new NodeT<T>(n);

	// increase the queue size
	++currentSize;

	// check if the queue is empty
	// if true then set the front and back pointer to the first node in the queue
	if (empty())
	{
		frontPtr = backPtr = newNode;
		return;
	}

	// otherwise if there exists a node in the queue insert it in to the back
	backPtr->next = newNode;
	backPtr = newNode;
}

template<class T>
T QueueT<T>::dequeue()
{
	// If queue is empty throw runtime error
	if (empty()) { throw std::runtime_error("List is empty"); }

	// create a T type to store the value of the dequeued node
	T value;

	// decrement queue size
	--currentSize;

	// store the current front and then assign the front->next as the front of the queue
	NodeT<T>* prevNode = frontPtr;
	frontPtr = frontPtr->next;

	// If front is null change the back to null aswell
	if (frontPtr == nullptr) { backPtr = nullptr; }

	// assign the dequeued node data before de-allocating
	value = prevNode->data;

	// deallocate the old front
	delete prevNode;

	return value;
}

template<class T>
void QueueT<T>::print() const
{
	// Queue iterator 
	NodeT<T>* current = frontPtr;

	// loops through the queue and prints out the data
	for (int i = 0; i < currentSize; i++)
	{
		// checks if it isn't null
		if (current != nullptr)
		{
			LOG(current->data);
			current = current->next;
		}
	}

	// when the iterator is not needed anymore delete it
	delete current;
}

template<class T>
bool QueueT<T>::empty() const
{
	// checks if the front is null
	isPointerNull(frontPtr, true);

	// otherwise return false
	return false;
}

template<class T>
int QueueT<T>::size() const
{
	// returns queue size
	return currentSize;
}

template<class T>
void QueueT<T>::removeAll()
{
	// as long as the queue is not empty dequeue the queue
	while (!empty())
	{
		// call deqeue
		dequeue();
	}
}

template<class T>
void QueueT<T>::concatenate(QueueT<T>& concat, int n)
{

	// checks if the param front is null
	isPointerNull(concat.frontPtr);

	// checks if the param size is greater than n, if so throw a runtime err
	if (concat.size() > n) { throw std::runtime_error("out of bounds"); }

	// add the n to currentsize
	currentSize += n;

	// loop through and add concat to the calling obj
	for (int i = 0; i < n; i++)
	{
		// calls enqueue and sends the concat param data
		// then dequeues the data from concat
		//double checks if not null
		if (concat.frontPtr != nullptr)
		{
			enqueue(concat.frontPtr->data);
			concat.dequeue();
		}

	}

}

template<class T>
QueueT<T> const QueueT<T>::merge(const QueueT<T>& mergeList)
{
	// checks if the param front is null
	isPointerNull(mergeList.frontPtr, *this);

	// checks if the front is null
	isPointerNull(frontPtr, mergeList);

	// create two iterators on for the calling object and the other for the param
	NodeT<T>* currObj = frontPtr;
	NodeT<T>* currParam = mergeList.frontPtr;

	// create a new queue
	QueueT<T> newQueue;

	// loop through the queue 
	while (currObj != nullptr || currParam != nullptr)
	{
		// call enqueue and copies the data to the new queue
		if (currObj != nullptr)
		{
			newQueue.enqueue(currObj->data);
			currObj = currObj->next;
		}

		// same process above but with the param
		if (currParam != nullptr)
		{
			newQueue.enqueue(currParam->data);
			currParam = currParam->next;
		}
	}

	// return the new queue
	return newQueue;
}

template<class T>
NodeT<T>* QueueT<T>::getFront()
{
	// returns the front pointer
	return frontPtr;
}