#pragma once
class Graph {
public:
    Graph(unsigned int width, unsigned int height, std::string name)
        : window(sf::VideoMode(width, height), name), windowWidth(width), windowHeight(height), resolutionSet(false) {

        this->name = name; // Assign the name

        if (!font.loadFromFile("DejaVuSansMono-Bold.ttf")) {
            std::cerr << "Error loading font\n";
            // Handle error - you might want to exit the program or use a default font
        }

        text.setFont(font);
        text.setCharacterSize(16); // Example size
        text.setFillColor(sf::Color::Blue);
    }

    void setPoints(const std::vector<double>& xValues, const std::vector<double>& yValues) {
        points.clear();
        for (size_t i = 0; i < xValues.size(); ++i) {
            points.emplace_back(xValues[i], yValues[i]);
        }
    }

    void setResolution(double xRes, double yRes) {
        xResolution = xRes;
        yResolution = yRes;
        resolutionSet = true; // Mark resolution as manually set
    }

    void setXAxisHeight(double height) {
        xAxisHeight = height;
    }

    void setYAxisPosition(double position) {
        yAxisPosition = 1 - position;
    }
    
    void setAutoResolution(double zoom_multiplier = 1) {//increase to zoom out
        if (points.empty()) return;

        double xMin = points.front().first;
        double xMax = points.front().first;
        double yMin = points.front().second;
        double yMax = points.front().second;

        for (const auto& point : points) {
            if (point.first < xMin) xMin = point.first;
            if (point.first > xMax) xMax = point.first;
            if (point.second < yMin) yMin = point.second;
            if (point.second > yMax) yMax = point.second;
        }

        double xRange = xMax - xMin;
        double yRange = yMax - yMin;

        if (yAxisPosition == 0.5 && xAxisHeight == 0.5) {
            // When both axes are centered, use the entire window space
            xResolution = (((window.getSize().x * 0.95) / xRange) > 2000 || ((window.getSize().x * 0.95) / xRange) < 0.1) ? 90 : (window.getSize().x * 0.95) / xRange;
            yResolution = (((window.getSize().y * 0.95) / yRange) > 2000 || ((window.getSize().y * 0.95) / yRange) < 0.1) ? 150 : (window.getSize().y * 0.95) / yRange;
        }
        else {

            // Proportion of window allocated to negative and positive x values
            double negXProp = (xMin < 0) ? -xMin / xRange : 0;
            double posXProp = (xMax > 0) ? xMax / xRange : 0;

            // Proportion of window allocated to negative and positive y values
            double negYProp = (yMin < 0) ? -yMin / yRange : 0;
            double posYProp = (yMax > 0) ? yMax / yRange : 0;

            // Adjust resolutions based on allocated proportions
            xResolution = window.getSize().x / (xRange * (negXProp / yAxisPosition + posXProp / (1 - yAxisPosition)));
            if (xResolution > 2000 || xResolution < 0.1) {
                xResolution = 45;
            }else {
                xResolution = xResolution * zoom_multiplier;
            }
            yResolution = window.getSize().y / (yRange * (negYProp / xAxisHeight + posYProp / (1 - xAxisHeight)));
            if (yResolution > 2000 || yResolution < 0.1) {
                yResolution = 90;
            }else {
                yResolution = yResolution * zoom_multiplier;
            }
            // Adjust offsets
            xOffset = xMin;
            yOffset = yMin;

        }
    }

    void drawXAxis() {
        // Calculate the y position of the x-axis (which corresponds to y=0 in graph coordinates)
        float xAxisYPosition = static_cast<float>(windowHeight * (1 - xAxisHeight));

        // Create a horizontal line shape for the x-axis
        sf::RectangleShape xAxis(sf::Vector2f(static_cast<float>(windowWidth), 1.0f));
        xAxis.setPosition(0, xAxisYPosition);
        xAxis.setFillColor(sf::Color::White);

        window.draw(xAxis);
    }

    void drawYAxis() {
        // Calculate the x position of the y-axis (which corresponds to x=0 in graph coordinates)
        float yAxisXPosition = static_cast<float>(windowWidth * (1 - yAxisPosition));

        // Create a vertical line shape for the y-axis
        sf::RectangleShape yAxis(sf::Vector2f(1.0f, static_cast<float>(windowHeight)));
        yAxis.setPosition(yAxisXPosition, 0);
        yAxis.setFillColor(sf::Color::White);

        window.draw(yAxis);
    }

    void draw() {
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }

            window.clear(sf::Color::Black);
            drawXAxis();
            drawYAxis();
            drawPoints();
            window.display();
        }
    }

    void print_variables() {
        cout << name << " x resolution: " << xResolution << endl;
        cout << name << " y resolution: " << yResolution << endl;
        cout << name << " x Offset: " << xOffset << endl;
        cout << name << " y Offset: " << yOffset << endl;
        cout << name << " point.first: " << point_first << endl;
        cout << name << " point.second: " << point_second << endl;
        cout << name << " graphZeroX: " << graphZero_X << endl;
        cout << name << " graphZeroY: " << graphZero_Y << endl;
        cout << name << " windowHeight: " << windowHeight << endl;
        cout << name << " xAxisHeight: " << xAxisHeight << endl;
    }

private:
    std::string name;
    sf::RenderWindow window;
    std::vector<std::pair<double, double>> points;
    double xResolution = 1.0; // Default X resolution
    double yResolution = 1.0; // Default Y resolution
    double xOffset = 0.0; // Offset for x coordinates
    double yOffset = 0.0; // Offset for y coordinates
    //DEBUG VARIABLES
    double point_first, point_second, graphZero_Y, graphZero_X;

    unsigned int windowHeight, windowWidth;
    double xAxisHeight = 0.5; // X axis at the middle by default
    double yAxisPosition = 0.5; // Y axis position (x=0 line), default in the middle (0.5)
    bool resolutionSet;

    sf::Font font;
    sf::Text text;
    
    std::string generateResolutionString(double xRes, double yRes) {
        std::ostringstream ss;
        ss << "X resolution = " << xRes << "\n";
        ss << "Y resolution = " << yRes << "\n";
        ss << "Made with TommyLibrary";
        return ss.str();
    }

    void drawPoints() {
        double graphZeroY = windowHeight * (1 - xAxisHeight); // Position of y=0 in the graph
        double graphZeroX = windowWidth * (1 - yAxisPosition); // Position of x=0 in the graph

        for (const auto& point : points) {
            sf::CircleShape shape(2); // Small circle for each point

            // Set color based on x and y values
            if (point.first < 0 && point.second >= 0) {
                shape.setFillColor(sf::Color::Blue); // Blue for x negative and y positive
            }
            else if (point.first < 0 && point.second < 0) {
                shape.setFillColor(sf::Color(128, 64, 255)); // Color between blue and purple for both x and y negative
            }
            else if (point.second >= 0) {
                shape.setFillColor(sf::Color::Red); // Red for y positive (also x positive)
            }
            else {
                shape.setFillColor(sf::Color(128, 0, 128)); // Purple-red color for y negative (also x positive)
            }

            //for debug
            point_first = point.first; point_second = point.second; graphZero_Y = graphZeroY;
            // Adjusted coordinate transformation
            //float x = static_cast<float>((point.first - xOffset) * xResolution); 
            // Calculate y-position based on xAxisHeight and also x-position based on yAxisPosition
            float x = static_cast<float>(graphZeroX + (point.first * xResolution));
            float y = static_cast<float>(graphZeroY - (point.second * yResolution));

            shape.setPosition(x, y);
            window.draw(shape);

            //TEXT

            // Position the text in the bottom-right corner of the graph
            text.setString(generateResolutionString(xResolution,yResolution));
            sf::FloatRect textRect = text.getLocalBounds();
            text.setPosition(windowWidth - textRect.width - 10, windowHeight - textRect.height - 10);
            window.draw(text); // Draw the text
        }
    }
    
};

/*

// Create a window with the same pixel depth as the desktop
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");

    // Main loop that continues until we close the window
    while (window.isOpen()) {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            // If a key is pressed, close the window
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Clear the window to black (or any other color)
        window.clear(sf::Color::Black);

        // You can draw other shapes and entities here
        // For example, let's draw a simple circle
        sf::CircleShape circle(50);  // radius 50
        circle.setFillColor(sf::Color::Red);  // fill color red
        circle.setPosition(200, 200);  // position the circle

        window.draw(circle);  // draw the circle

        // Display what was drawn (swap buffers)
        window.display();
    }

*/