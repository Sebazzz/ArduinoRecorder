#pragma once

/** Custom vector class - automatically expanding array */
template <class T> class Vector
{
private:
	T* _items;
	unsigned short _index;
	unsigned short _size;

	static const unsigned short SIZE_INCREASE = 1;
	static const unsigned short SIZE_INITIAL = 2;
	static const unsigned short SIZE = sizeof(T);

public:
	Vector() {
		this->_items = new T[SIZE_INITIAL];
		this->_index = 0;
		this->_size = SIZE_INITIAL;
	}

	Vector(const Vector<T>& other) {
		this->_index = other._index;
		this->_size = other._size;
		
		this->_items = new T[other._size];
		memcpy(this->_items, other._items, other._size * sizeof(T));
	}

	~Vector() {
		delete this->_items;
	}

	void clear(bool reduceSize) {
		this->_index = 0;

		if (reduceSize) {
			delete this->_items;

			this->_items = new T[SIZE_INITIAL];
			this->_size = SIZE_INITIAL;
		}
	}

	T getItem(unsigned short index) const {
		T* pointer = this->_items + index;
		return *pointer;
	}

	inline unsigned short capacity() const {
		return this->_size;
	}

	inline unsigned short size() const {
		return this->_index;
	}

	void addItem(T item) {
		if (this->_index >= this->_size) {
			// need to resize array
			unsigned short newSize = this->_size + Vector<T>::SIZE_INCREASE;

			T* newArr = new T[newSize];
			memcpy(newArr, this->_items, this->_size * sizeof(T));
			delete this->_items;
			this->_items = newArr;
			this->_size = newSize;
		}
		
		*(this->_items + this->_index) = item;
		this->_index++;
	}
};

