# Задание по программированию «Интеграция структуры Point в unordered_set»

Мы хотим научиться встраивать структуру Point3D в контейнер unordered_set<Point3D, Hasher>. Для этого вам нужно реализовать структуру Hasher и оператор сравнения:

```C++
using CoordType = int;

struct Point3D {
  CoordType x;
  CoordType y;
  CoordType z;

  bool operator==(const Point3D& other) const {
    // реализуйте оператор
  }
};

struct Hasher {
  // реализуйте структуру
};
```

В своей реализации используйте стандартную хеш-функцию std::hash<CoordType> для хеширования координат и комбинируйте полученные хеши с помощью полинома, как показано в лекции.

## Требования:

- хеш-функция должна зависеть от всех координат точки
- хеш-функция должна равномерно рассеивать точки в трёхмерном пространстве; это её свойство проверяется тестом TestDistribution в заготовке решения.

## Заготовка решения

[hash_point.cpp](https://stepik.org/media/attachments/lesson/287654/hash_point.cpp)
