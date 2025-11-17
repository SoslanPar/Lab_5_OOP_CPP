#include <gtest/gtest.h>
#include "../include/memory_resource.h"
#include <memory_resource>

// Тест 1: Создание memory_resource
TEST(MemoryResourceTest, Construction) {
    EXPECT_NO_THROW({
        CustomMemoryResource mr(1024);
    });
}

// Тест 2: Выделение памяти
TEST(MemoryResourceTest, BasicAllocation) {
    CustomMemoryResource mr(1024);
    
    void* ptr1 = mr.allocate(64, alignof(int));
    EXPECT_NE(ptr1, nullptr);
    
    void* ptr2 = mr.allocate(32, alignof(double));
    EXPECT_NE(ptr2, nullptr);
    EXPECT_NE(ptr1, ptr2); // Разные блоки
}

// Тест 3: Выравнивание
TEST(MemoryResourceTest, Alignment) {
    CustomMemoryResource mr(1024);
    
    void* ptr_int = mr.allocate(sizeof(int), alignof(int));
    uintptr_t addr_int = reinterpret_cast<uintptr_t>(ptr_int);
    EXPECT_EQ(addr_int % alignof(int), 0);
    
    void* ptr_double = mr.allocate(sizeof(double), alignof(double));
    uintptr_t addr_double = reinterpret_cast<uintptr_t>(ptr_double);
    EXPECT_EQ(addr_double % alignof(double), 0);
}

// Тест 4: Освобождение и переиспользование памяти
TEST(MemoryResourceTest, DeallocationAndReuse) {
    CustomMemoryResource mr(1024);
    
    void* ptr1 = mr.allocate(64, alignof(int));
    mr.deallocate(ptr1, 64, alignof(int));
    
    // Выделяем блок того же размера - должен переиспользоваться
    void* ptr2 = mr.allocate(64, alignof(int));
    EXPECT_EQ(ptr1, ptr2); // Должны быть одинаковыми (переиспользование)
}

// Тест 5: Недостаточно памяти
TEST(MemoryResourceTest, OutOfMemory) {
    CustomMemoryResource mr(128); // Маленький пул
    
    EXPECT_NO_THROW({
        mr.allocate(64, alignof(int));
    });
    
    // Пытаемся выделить больше, чем осталось
    EXPECT_THROW({
        mr.allocate(128, alignof(int));
    }, std::bad_alloc);
}

// Тест 6: Множественные выделения и освобождения
TEST(MemoryResourceTest, MultipleAllocations) {
    CustomMemoryResource mr(2048);
    
    std::vector<void*> pointers;
    
    // Выделяем несколько блоков
    for (int i = 0; i < 10; ++i) {
        void* ptr = mr.allocate(64, alignof(int));
        EXPECT_NE(ptr, nullptr);
        pointers.push_back(ptr);
    }
    
    // Освобождаем все
    for (void* ptr : pointers) {
        EXPECT_NO_THROW({
            mr.deallocate(ptr, 64, alignof(int));
        });
    }
}

// Тест 7: Статистика памяти
TEST(MemoryResourceTest, MemoryStats) {
    CustomMemoryResource mr(1024);
    
    EXPECT_EQ(mr.get_used_memory(), 0);
    EXPECT_EQ(mr.get_free_memory(), 1024);
    
    mr.allocate(100, alignof(int));
    EXPECT_GT(mr.get_used_memory(), 0);
    EXPECT_LT(mr.get_free_memory(), 1024);
}

// Тест 8: do_is_equal
TEST(MemoryResourceTest, Equality) {
    CustomMemoryResource mr1(1024);
    CustomMemoryResource mr2(1024);
    
    EXPECT_TRUE(mr1.is_equal(mr1));
    EXPECT_FALSE(mr1.is_equal(mr2)); // Разные объекты
}

// Тест 9: Неправильный deallocate
TEST(MemoryResourceTest, InvalidDeallocate) {
    CustomMemoryResource mr(1024);
    
    void* fake_ptr = reinterpret_cast<void*>(0x12345678);
    
    EXPECT_THROW({
        mr.deallocate(fake_ptr, 64, alignof(int));
    }, std::invalid_argument);
}

// Тест 10: Разные размеры блоков
TEST(MemoryResourceTest, DifferentSizes) {
    CustomMemoryResource mr(4096);
    
    void* ptr_small = mr.allocate(8, alignof(char));
    void* ptr_medium = mr.allocate(64, alignof(int));
    void* ptr_large = mr.allocate(512, alignof(double));
    
    EXPECT_NE(ptr_small, nullptr);
    EXPECT_NE(ptr_medium, nullptr);
    EXPECT_NE(ptr_large, nullptr);
    
    // Все должны быть разными
    EXPECT_NE(ptr_small, ptr_medium);
    EXPECT_NE(ptr_medium, ptr_large);
    EXPECT_NE(ptr_small, ptr_large);
}