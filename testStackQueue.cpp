#define CATCH_CONFIG_MAIN
#include <iostream>
#include "util/catch.hpp"
#include "stack.h"
#include "queue.h"
//using namespace util;
using namespace std;

TEST_CASE("stack::basic functions","[weight=1][part=stack]"){
    //cout << "Testing Stack..." << endl;
    Stack<int> intStack;
    vector<int> result;
    vector<int> expected;
    for (int i = 10; i > 0; i--) {
        expected.push_back(i);
    }
    for (int i = 1; i <= 10; i++) {
        intStack.push(i);
    }
    while (!intStack.isEmpty()) {
        result.push_back(intStack.pop());
    }
    REQUIRE( result == expected);
}
TEST_CASE("queue::basic functions","[weight=1][part=queue]"){
    Queue<int> intQueue;
    vector<int> result;
    vector<int> expected;
    for (int i = 1; i <= 10; i++) {
        expected.push_back(i);
    }
    for (int i = 1; i <= 10; i++) {
        intQueue.enqueue(i);
    }
    while (!intQueue.isEmpty()) {
        result.push_back(intQueue.dequeue());
    }
    REQUIRE( result == expected);
}

TEST_CASE("Deque::basic functions","[weight=1][part=deque]"){
    Deque<int> intDeque;
    vector<int> result;
    vector<int> expected;
    for (int i = 1; i <= 10; i++) {
        intDeque.pushR(i);
    }
    for (int i = 1; i <= 7; i++) {
        intDeque.popL();
    }
    while (!intDeque.isEmpty()) {
        result.push_back(intDeque.popR());
    }
    expected.push_back(10);
    expected.push_back(9);
    expected.push_back(8);
    REQUIRE( result == expected);
}

