#include <gtest/gtest.h>
#include "../include/list.h"
#include "../include/memory_resource.h"

// Тест 1: Создание списка
TEST(DoublyLinkedListTest, Construction) {
    CustomMemoryResource mr(1024);
    
    EXPECT_NO_THROW({
        list<int> list(&mr);
    });
}

// Тест 2: Push back
TEST(DoublyLinkedListTest, PushBack) {
    CustomMemoryResource mr(1024);
    list<int> list(&mr);
    
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.size(), 0);
    
    list.push_back(10);
    EXPECT_FALSE(list.empty());
    EXPECT_EQ(list.size(), 1);
    
    list.push_back(20);
    list.push_back(30);
    EXPECT_EQ(list.size(), 3);
}

// Тест 3: Push front
TEST(DoublyLinkedListTest, PushFront) {
    CustomMemoryResource mr(1024);
    list<int> list(&mr);
    
    list.push_front(10);
    list.push_front(20);
    list.push_front(30);
    
    EXPECT_EQ(list.size(), 3);
}

// Тест 4: Pop back
TEST(DoublyLinkedListTest, PopBack) {
    CustomMemoryResource mr(1024);
    list<int> list(&mr);
    
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);
    
    EXPECT_EQ(list.size(), 3);
    
    list.pop_back();
    EXPECT_EQ(list.size(), 2);
    
    list.pop_back();
    EXPECT_EQ(list.size(), 1);
    
    list.pop_back();
    EXPECT_EQ(list.size(), 0);
    EXPECT_TRUE(list.empty());
}

// Тест 5: Pop front
TEST(DoublyLinkedListTest, PopFront) {
    CustomMemoryResource mr(1024);
    list<int> list(&mr);
    
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);
    
    list.pop_front();
    EXPECT_EQ(list.size(), 2);
    
    list.pop_front();
    list.pop_front();
    EXPECT_TRUE(list.empty());
}

// Тест 6: Pop из пустого списка
TEST(DoublyLinkedListTest, PopEmptyList) {
    CustomMemoryResource mr(1024);
    list<int> list(&mr);
    
    EXPECT_THROW(list.pop_back(), std::out_of_range);
    EXPECT_THROW(list.pop_front(), std::out_of_range);
}

// Тест 7: Clear
TEST(DoublyLinkedListTest, Clear) {
    CustomMemoryResource mr(1024);
    list<int> list(&mr);
    
    for (int i = 0; i < 10; ++i) {
        list.push_back(i);
    }
    
    EXPECT_EQ(list.size(), 10);
    
    list.clear();
    EXPECT_EQ(list.size(), 0);
    EXPECT_TRUE(list.empty());
}

// Тест 8: Итератор - базовый проход
TEST(DoublyLinkedListTest, IteratorBasic) {
    CustomMemoryResource mr(1024);
    list<int> list(&mr);
    
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);
    
    std::vector<int> values;
    for (auto it = list.begin(); it != list.end(); ++it) {
        values.push_back(*it);
    }
    
    EXPECT_EQ(values.size(), 3);
    EXPECT_EQ(values[0], 10);
    EXPECT_EQ(values[1], 20);
    EXPECT_EQ(values[2], 30);
}

// Тест 9: Range-based for loop
TEST(DoublyLinkedListTest, RangeBasedFor) {
    CustomMemoryResource mr(1024);
    list<int> list(&mr);
    
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_back(4);
    list.push_back(5);
    
    int sum = 0;
    for (int value : list) {
        sum += value;
    }
    
    EXPECT_EQ(sum, 15); // 1+2+3+4+5 = 15
}

// Тест 10: Итератор - постфиксный инкремент
TEST(DoublyLinkedListTest, IteratorPostIncrement) {
    CustomMemoryResource mr(1024);
    list<int> list(&mr);
    
    list.push_back(10);
    list.push_back(20);
    
    auto it = list.begin();
    auto old_it = it++;
    
    EXPECT_EQ(*old_it, 10);
    EXPECT_EQ(*it, 20);
}

// Тест 11: Пустой список и итераторы
TEST(DoublyLinkedListTest, EmptyListIterator) {
    CustomMemoryResource mr(1024);
    list<int> list(&mr);
    
    EXPECT_EQ(list.begin(), list.end());
}

// Тест 12: Большое количество элементов
TEST(DoublyLinkedListTest, LargeList) {
    CustomMemoryResource mr(1024 * 1024); // 1 MB
    list<int> list(&mr);
    
    const int N = 1000;
    for (int i = 0; i < N; ++i) {
        list.push_back(i);
    }
    
    EXPECT_EQ(list.size(), N);
    
    int count = 0;
    for (int value : list) {
        EXPECT_EQ(value, count);
        ++count;
    }
}

// Тест 13: Работа с double
TEST(DoublyLinkedListTest, DoubleType) {
    CustomMemoryResource mr(1024);
    list<double> list(&mr);
    
    list.push_back(3.14);
    list.push_back(2.71);
    list.push_back(1.41);
    
    auto it = list.begin();
    EXPECT_DOUBLE_EQ(*it, 3.14);
    ++it;
    EXPECT_DOUBLE_EQ(*it, 2.71);
    ++it;
    EXPECT_DOUBLE_EQ(*it, 1.41);
}

// Тест 14: Переиспользование памяти
TEST(DoublyLinkedListTest, MemoryReuse) {
    CustomMemoryResource mr(2048);
    list<int> list(&mr);
    
    size_t used_before = mr.get_used_memory();
    
    // Добавляем элементы
    for (int i = 0; i < 10; ++i) {
        list.push_back(i);
    }
    
    size_t used_after_push = mr.get_used_memory();
    EXPECT_GT(used_after_push, used_before);
    
    // Удаляем элементы
    list.clear();
    
    // Добавляем снова - память должна переиспользоваться
    for (int i = 0; i < 10; ++i) {
        list.push_back(i * 2);
    }
    
    size_t used_after_reuse = mr.get_used_memory();
    
    // Память не должна сильно вырасти (переиспользование)
    EXPECT_LE(used_after_reuse, used_after_push + 100); // Небольшой запас на выравнивание
}