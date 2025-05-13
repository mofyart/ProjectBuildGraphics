// // Функция для вычисления значения функции в заданной точке x
// double getFunc() {
//     // Объявление переменной для хранения математического выражения
//     std::string expression;
//     // Объявление переменной для значения x
//     double x;

//     // Запрос ввода математического выражения у пользователя
//     std::cout << "Введите математическое выражение (например, sin(x^2) + cos(x)): ";
//     std::getline(std::cin, expression);

//     // Запрос ввода значения x у пользователя
//     std::cout << "Введите значение x: ";
//     std::cin >> x;
//     // Очистка буфера ввода после чтения числа
//     std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

//     // Создание символической таблицы для exprtk
//     exprtk::symbol_table<double> symbol_table;
//     // Добавление переменной 'x' в символическую таблицу
//     symbol_table.add_variable("x", x);

//     // Создание объекта выражения
//     exprtk::expression<double> expr;
//     // Регистрация символической таблицы в выражении
//     expr.register_symbol_table(symbol_table);

//     // Создание парсера для компиляции выражения
//     exprtk::parser<double> parser;
//     // Компиляция математического выражения
//     if (!parser.compile(expression, expr)) {
//         // Вывод ошибки компиляции, если она произошла
//         std::cerr << "Ошибка компиляции: " << parser.error() << std::endl;
//         // Возврат значения по умолчанию или индикатора ошибки
//         return 1.; // Возвращаем 1. как индикатор ошибки компиляции
//     }

//     // Вычисление значения выражения при заданном x
//     double result = expr.value();
//     // Вывод результата
//     std::cout << "Результат: y = " << result << std::endl;

//     // Возвращаем вычисленное значение
//     return result;
// }



//   // // Обработка движения мыши во время перетаскивания
            // if (isMouseDragging && event.type == sf::Event::MouseMoved) {
            //     sf::Vector2i currentMousePos = sf::Mouse::getPosition(window);
            //     sf::Vector2f worldDelta = window.mapPixelToCoords(initialMousePos, view) - window.mapPixelToCoords(currentMousePos, view);
            //     sf::Vector2f newCenter = view.getCenter() + worldDelta;

            //     // Проверка и коррекция ограничений вручную
            //     sf::Vector2f totalDeltaFromInitial = newCenter - initialViewCenter;
            //      if (std::abs(totalDeltaFromInitial.x) > movementLimit.x) {
            //         newCenter.x = initialViewCenter.x + (totalDeltaFromInitial.x > 0 ? movementLimit.x : -movementLimit.x);
            //     }
            //     if (std::abs(totalDeltaFromInitial.y) > movementLimit.y) {
            //         newCenter.y = initialViewCenter.y + (totalDeltaFromInitial.y > 0 ? movementLimit.y : -movementLimit.y);
            //     }

            //     view.setCenter(newCenter);
            //     initialMousePos = currentMousePos;

            // }
