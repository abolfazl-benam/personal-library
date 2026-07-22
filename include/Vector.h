#ifndef VECTOR_H
#define VECTOR_H

template <typename T>
class Vector {
private:
    T* data;
    int capacity;
    int count;

    void resize() {
        int newCapacity = capacity * 2;
        T* newData = new T[newCapacity];
        for (int i = 0; i < count; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    Vector() {
        capacity = 4;
        count = 0;
        data = new T[capacity];
    }

    Vector(const Vector<T>& other) {
        capacity = other.capacity;
        count = other.count;
        data = new T[capacity];
        for (int i = 0; i < count; i++) {
            data[i] = other.data[i];
        }
    }

    ~Vector() {
        delete[] data;
    }

    Vector<T>& operator=(const Vector<T>& other) {
        if (this != &other) {
            delete[] data;
            capacity = other.capacity;
            count = other.count;
            data = new T[capacity];
            for (int i = 0; i < count; i++) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    void add(const T& value) {
        if (count == capacity) {
            resize();
        }
        data[count] = value;
        count++;
    }

    T& get(int index) {
        if (index < 0 || index >= count) {
            throw "Index out of range";
        }
        return data[index];
    }

    T get(int index) const {
        if (index < 0 || index >= count) {
            throw "Index out of range";
        }
        return data[index];
    }

    void set(int index, const T& value) {
        if (index < 0 || index >= count) {
            throw "Index out of range";
        }
        data[index] = value;
    }

    void removeAt(int index) {
        if (index < 0 || index >= count) {
            throw "Index out of range";
        }
        for (int i = index; i < count - 1; i++) {
            data[i] = data[i + 1];
        }
        count--;
    }

    int len() const {
        return count;
    }

    bool isEmpty() const {
        return count == 0;
    }

    void clear() {
        count = 0;
    }

    T& operator[](int index) {
        return get(index);
    }

    T operator[](int index) const {
        return get(index);
    }
};

#endif
