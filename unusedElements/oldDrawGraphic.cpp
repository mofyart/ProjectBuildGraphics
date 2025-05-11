//68 std::vector<sf::RectangleShape> graph;


    // for (double x = -wholeWidthWindow; x < wholeWidthWindow; x += 0.1) {
    //     double realX = (x - 700) / scaleX;

    //     // Создание символической таблицы
    //     exprtk::symbol_table<double> symbol_table;
    //     symbol_table.add_variable("x", realX);

    //     // Создание выражения
    //     exprtk::expression<double> expr;
    //     expr.register_symbol_table(symbol_table);

    //     // Компиляция выражения
    //     exprtk::parser<double> parser;
    //     if (!parser.compile(expression, expr)) {
    //         std::cerr << "Ошибка компиляции: " << parser.error() << std::endl;
    //         return;
    //     }

    //     // создание точки
    //     sf::RectangleShape point(sf::Vector2f(3.0f, 3.0f));

    //     // поиск знчения y
    //     float realY = expr.value();

    //     // определяем позицию точки
    //     point.setPosition(x - 1.5, 400 - realY * scaleY - 1.5);
    //     point.setFillColor(sf::Color::Green);

    //     // добавляем точку в вектор
    //     graph.push_back(point);
    // }
