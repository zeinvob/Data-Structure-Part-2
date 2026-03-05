#ifndef MYSTACK_HPP
#define MYSTACK_HPP

template<typename T, int MAX_SIZE = 100>
class MyStack {
    T data[MAX_SIZE];
    int topIdx;
public:
    MyStack() : topIdx(-1) {}
    bool isEmpty() const { return topIdx == -1; }
    bool isFull() const { return topIdx == MAX_SIZE - 1; }
    bool push(const T& val) {
        if (isFull()) return false;
        data[++topIdx] = val;
        return true;
    }
    bool pop() {
        if (isEmpty()) return false;
        --topIdx;
        return true;
    }
    T& top() { return data[topIdx]; }
    const T& top() const { return data[topIdx]; }
    int size() const { return topIdx + 1; }
};

#endif // MYSTACK_HPP
