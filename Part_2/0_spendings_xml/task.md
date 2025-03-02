# Задание по программированию «Загрузка расходов из формата XML»

В этой тренировочной задаче вам надо поддержать загрузку расходов из формата XML в нашей программе управления личными финансами. При этом вам надо воспользоваться готовой библиотекой работы с XML. Более конкретно, вам надо написать функцию `vector<Spending> LoadFromXml(istream& input)`. Spending — это структура из вводной лекции:

```C++
struct Spending {
  string category;
  int amount;
};
```

Поток input содержит описание расходов в формате XML, например:

```xml
<july>
  <spend amount="2500" category="food"></spend>
  <spend amount="1150" category="transport"></spend>
  <spend amount="5780" category="restaurants"></spend>
  <spend amount="7500" category="clothes"></spend>
  <spend amount="23740" category="travel"></spend>
  <spend amount="12000" category="sport"></spend>
</july>
```

Важно отметить:

- формат текста в потоке input всегда строго такой, как в примере: есть один корневой узел, который содержит несколько узлов `<spend>` с атрибутами "amount" и "category";
- функция Load из выданной вам библиотеки работы с XML умеет загружать описанный выше формат;
- мы выдаём вам не реальную библиотеку разбора XML, а лишь минимальный код, достаточный для работы описанным форматом данных;
- цель этой задачи — не показать, как правильно парсить XML, а познакомить вас с кодом, на котором мы в дальнейшем будем разбирать пространства имён, поэтому очень важно, чтобы вы реализовали функцию LoadFromXml через содержимое нашей библиотеки работы с XML.

На проверку пришлите cpp-файл, который подключает файл xml.h (дан ниже) и содержит реализацию функции LoadFromXml. Тестирующая система соберёт его вместе с файлом xml.cpp и прогонит набор юнит-тестов.

## Заготовка решения и библиотека

- [xml](https://stepik.org/media/attachments/lesson/288021/X1cp9fexEeilxxL_ZeRz_A_5fb0bb50f7b111e8a2f4f31ac9657a8d_xml.h)
- [xml.cpp](https://stepik.org/media/attachments/lesson/288021/X1Y_7fexEeiAgQrXx6bp4g_5f8f50a0f7b111e8834415d2586e2901_xml.cpp)
- [spendings_xml](https://stepik.org/media/attachments/lesson/288021/X1Xxs_exEeiixgqCUDoEfA_5f73d960f7b111e894354b825341e8a4_spendings_xml.cpp)
