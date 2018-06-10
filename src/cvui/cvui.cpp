#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "./cvui/cvui.h"

static void group(cv::Mat& frame, int x, int y, int width, int height) {
    int padding = 5, w = (width - padding) / 4, h = (height - 15 - padding) / 2;
    cv::Point pos(x + padding, y + padding);
    
    cvui::text(frame, pos.x, pos.y, "Group title");
    pos.y += 15;
    
    cvui::window(frame, pos.x, pos.y, width - padding * 2, h - padding, "Something");
    // `0xff000000` for transparent filling, see cvui.h (noted by me)
    cvui::rect(frame, pos.x + 2, pos.y + 20, width - padding * 2 - 5, h - padding - 20, 0xff0000);
    pos.y += h;
    
    cvui::window(frame, pos.x, pos.y, w / 3 - padding, h, "Some");
    cvui::text(frame, pos.x + 25, pos.y + 60, "65", 1.1);
    pos.x += w / 3;
    
    cvui::window(frame, pos.x, pos.y, w / 3 - padding, h, "Info");
    cvui::text(frame, pos.x + 25, pos.y + 60, "30", 1.1);
    pos.x += w / 3;
    
    cvui::window(frame, pos.x, pos.y, w / 3 - padding, h, "Here");
    cvui::text(frame, pos.x + 25, pos.y + 60, "70", 1.1);
    pos.x += w / 3;
    
    cvui::window(frame, pos.x, pos.y, w - padding, h, "And");
    cvui::rect(frame, pos.x + 2, pos.y + 22, w - padding - 5, h - padding - 20, 0xff0000);
    pos.x += w;
    
    cvui::window(frame, pos.x, pos.y, w - padding, h, "Here");
    cvui::rect(frame, pos.x + 2, pos.y + 22, w - padding - 5, h - padding - 20, 0xff0000);
    pos.x += w;
    
    cvui::window(frame, pos.x, pos.y, w - padding, h, "More info");
    cvui::rect(frame, pos.x + 2, pos.y + 22, w - padding - 5, h - padding - 20, 0xff0000);
    pos.x += w;
}

// Update a window using cvui functions, then show it using cv::imshow().
static void window(const cv::String& name) {
	// Create a frame for this window and fill it with a nice color
	cv::Mat frame = cv::Mat(200, 500, CV_8UC3);
	frame = cv::Scalar(49, 52, 49);

	// Inform cvui that the components to be rendered from now one belong to
	// a window in particular.
	//
	// If you don't inform that, cvui will assume the components belong to
	// the default window (informed in cvui::init()). In that case, the
	// interactions with all other windows being used will not work.
	cvui::context(name);

	// Show info regarding the window
	cvui::printf(frame, 110, 50, "%s - click the button", name.c_str());

	// Buttons return true if they are clicked
	if (cvui::button(frame, 110, 90, "Button")) {
		cvui::printf(frame, 200, 95, "Button clicked!");
		std::cout << "Button clicked on: " << name << std::endl;
	}

	// Tell cvui to update its internal structures regarding a particular window.
	//
	// If cvui is being used in multiple windows, you need to enclose all component
	// calls between the pair cvui::context(NAME)/cvui::update(NAME), where NAME is
	// the name of the window being worked on.
	cvui::update(name);

	// Show the content of this window on the screen
	cv::imshow(name, frame);
}

// Update and show a window in a single call using cvui::imshow().
static void compact(const cv::String& name) {
	// Create a frame for this window and fill it with a nice color
	cv::Mat frame = cv::Mat(200, 500, CV_8UC3);
	frame = cv::Scalar(49, 52, 49);

	// Inform cvui that the components to be rendered from now one belong to
	// a window in particular.
	//
	// If you don't inform that, cvui will assume the components belong to
	// the default window (informed in cvui::init()). In that case, the
	// interactions with all other windows being used will not work.
	cvui::context(name);

	cvui::printf(frame, 110, 50, "%s - click the button", name.c_str());
	if (cvui::button(frame, 110, 90, "Button")) {
		cvui::printf(frame, 200, 95, "Button clicked!");
		std::cout << "Button clicked on: " << name << std::endl;
	}

	// Tell cvui to update its internal structures regarding a particular window
	// then show it. Below we are using cvui::imshow(), which is cvui's version of
	// the existing cv::imshow(). They behave exactly the same, the only difference
	// is that cvui::imshow() will automatically call cvui::update(name) for you.
	cvui::imshow(name, frame);
}

// Check if an OpenCV window is open.
// From: https://stackoverflow.com/a/48055987/29827
static int isWindowOpen(const cv::String &name) {
	return cv::getWindowProperty(name, cv::WND_PROP_AUTOSIZE) != -1;
}

// Open a new OpenCV window and watch it using cvui
static void openWindow(const cv::String &name) {
	cv::namedWindow(name);
	cvui::watch(name);
}

// Open an OpenCV window
static void closeWindow(const cv::String &name) {
	cv::destroyWindow(name);

	// Ensure OpenCV window event queue is processed, otherwise the window
	// will not be closed.
	cv::waitKey(1);
}

static std::vector<double> load(std::string thePath) {
	std::vector<double> data;
	double time, value;
	std::ifstream file(thePath.c_str());

	if (!file)	{
		throw std::runtime_error("Unable to open file");
	}

	while (file >> time >> value) {
		data.push_back(value);
	}

	return data;
}

namespace _cvui {
void hello_world() {
    std::string WINDOW_NAME = "hello_world";
    // Tell cvui to init and create a window
    cvui::init(WINDOW_NAME);
    // Create a frame
    cv::Mat frame = cv::Mat(cv::Size(400, 200), CV_8UC3);

    while(true) {
        // clear the frame
        frame = cv::Scalar(49, 52, 49);
        // render a message in the frame at position (10, 15)
        cvui::text(frame, 10, 15, "Hello world!");
        // Update cvui internal stuff
        cvui::update();
        // Show window content
        cv::imshow(WINDOW_NAME, frame);
        if (cv::waitKey(20) == 27) {
            break;
        }
    }
}
void button_shortcut() {
    std::string WINDOW_NAME = "button_shortcut";
    cv::Mat frame = cv::Mat(cv::Size(650, 150), CV_8UC3);
    // Init cvui and tell it to use a value of 20 for cv::waitKey()
    // because we want to enable keyboard shortcut for
    // all components, e.g. button with label "&Quit".
    // If cvui has a value for waitKey, it will call
    // waitKey() automatically for us within cvui::update().
    cvui::init(WINDOW_NAME, 20);
    while (true) {
        frame = cv::Scalar(49, 52, 49);
        cvui::text(frame, 40, 40, "To exit this app click the button below or press Q (shortcut for the button below).");
        // Exit the application if the quit button was pressed.
        // It can be pressed because of a mouse click or because 
        // the user pressed the "q" key on the keyboard, which is
        // marked as a shortcut in the button label ("&Quit").
        if (cvui::button(frame, 300, 80, "&Quit")) {
            break;
        }
        // Since cvui::init() received a param regarding waitKey,
        // there is no need to call cv::waitKey() anymore. cvui::update()
        // will do it automatically.
        cvui::update();
        cv::imshow(WINDOW_NAME, frame);
    }
}
void canny() {
    std::string WINDOW_NAME = "canny";
    cv::Mat lena = cv::imread("./src/cvui/cvui/example/data/lena.jpg");
    cv::Mat frame = cv::Mat(cv::Size(lena.cols + 200, lena.rows), CV_8UC3);
    cv::Rect roi = cv::Rect(200, 0, lena.cols, lena.rows);
    lena.copyTo(frame(roi));
    // cv::Mat frame = lena.clone();
    int low_threshold = 50, high_threshold = 150;
    bool use_canny = false;
    cvui::init(WINDOW_NAME);
    while (true) {
        if (use_canny) {
            cv::Mat lena_gray;
            cv::cvtColor(lena, lena_gray, CV_BGR2GRAY);
            if (frame.channels() == 3) {
                cv::cvtColor(frame, frame, CV_BGR2GRAY);
            }
            cv::Canny(lena_gray, lena_gray, low_threshold, high_threshold, 3);
            lena_gray.copyTo(frame(roi));
        } else {
            if (frame.channels() == 1) {
                cv::cvtColor(frame, frame, CV_GRAY2BGR);
            }
            lena.copyTo(frame(roi));
        }
        // Render the settings window to house the checkbox
        // and the trackbars below.
        cvui::window(frame, 10, 20, 180, lena.rows - 40, "Settings");
        // Checkbox to enable/disable the use of Canny edge
        cvui::checkbox(frame, 20, 80, "Use Canny Edge", &use_canny);
        // Two trackbars to control the low and high threshold values
        // for the Canny edge algorithm.
        cvui::trackbar(frame, 15, 140, 165, &low_threshold, 5, 150);
        cvui::trackbar(frame, 15, 250, 165, &high_threshold, 80, 300);
        // This function must be called *AFTER* all UI components. It does
        // all the behind the scenes magic to handle mouse clicks, etc.
        cvui::update();
        // Show everything on the screen
        cv::imshow(WINDOW_NAME, frame);
        // Check if ESC was pressed
        if (cv::waitKey(30) == 27) {
            break;
        }
    }
}

void complex_layout() {
    std::string WINDOW_NAME = "complex_layout";
    int height = 220, spacing = 10;
    cv::Mat frame = cv::Mat(height * 3, 1300, CV_8UC3);
    
    // Init cvui and tell it to create a OpenCV window, i.e. cv::namedWindow(WINDOW_NAME).
    cvui::init(WINDOW_NAME);
    
    while (true) {
        // Fill the frame with a nice color
        frame = cv::Scalar(49, 52, 49);
        
        // Render three groups of components.
        group(frame, 0, 0, frame.cols, height - spacing);
        group(frame, 0, height, frame.cols, height - spacing);
        group(frame, 0, height * 2, frame.cols, height - spacing);
        
        // This function must be called *AFTER* all UI components. It does
        // all the behind the scenes magic to handle mouse clicks, etc.
        cvui::update();

        // Show everything on the screen
        cv::imshow(WINDOW_NAME, frame);
        
        // Check if ESC key was pressed
        if (cv::waitKey(20) == 27) {
            break;
        }
    }
}

void image_button() {
    std::string WINDOW_NAME = "image_button";
    cv::Mat frame = cv::Mat(120, 280, CV_8UC3);
    cv::Mat out = cv::imread("./src/cvui/data/btn_up.jpg", cv::IMREAD_COLOR);
    cv::Mat down = cv::imread("./src/cvui/data/btn_down.jpg", cv::IMREAD_COLOR);
    cv::Mat over = cv::imread("./src/cvui/data/btn_up.jpg", cv::IMREAD_COLOR);
    
    // Init cvui and tell it to create a OpenCV window, i.e. cv::namedWindow(WINDOW_NAME).
    cvui::init(WINDOW_NAME);
    
    while (true) {
        // Fill the frame with a nice color
        frame = cv::Scalar(255, 255, 255);
        
        // Render an image-based button. You can provide images
        // to be used to render the button when the mouse cursor is
        // outside, over or down the button area.
        cvui::text(frame, 10, 10, "image button", 0.5, 0xff0000);
        if (cvui::button(frame, 30, 40, out, over, down)) {
            std::cout << "Image button clicked!" << std::endl;
        }
        
        cvui::text(frame, 140, 10, "normal button", 0.5, 0xff0000);
        // Render a regular button.
        if (cvui::button(frame, 165, 55, "Button")) {
            std::cout << "Regular button clicked!" << std::endl;
        }
        
        // This function must be called *AFTER* all UI components. It does
        // all the behind the scenes magic to handle mouse clicks, etc.
        cvui::update();
        
        // Show everything on the screen
        cv::imshow(WINDOW_NAME, frame);
        
        // Check if ESC key was pressed
        if (cv::waitKey(20) == 27) {
        	break;
        }
    }
}

void interaction_area() {
    std::string WINDOW_NAME = "interaction_area";
    cv::Mat frame = cv::Mat(300, 600, CV_8UC3);
    
    // Init cvui and tell it to create a OpenCV window, i.e. cv::namedWindow(WINDOW_NAME).
    cvui::init(WINDOW_NAME);
    
    while (true) {
        // Fill the frame with a nice color
        frame = cv::Scalar(49, 52, 49);
        
        // Render a rectangle on the screen.
        cv::Rect rectangle(50, 50, 100, 100);
        cvui::rect(frame, rectangle.x, rectangle.y, rectangle.width, rectangle.height, 0xff0000);
        
        // Check what is the current status of the mouse cursor
        // regarding the previously rendered rectangle.
        int status = cvui::iarea(rectangle.x, rectangle.y, rectangle.width, rectangle.height);
        
        // cvui::iarea() will return the current mouse status:
        //  CLICK: mouse just clicked the interaction are
        //	DOWN: mouse button was pressed on the interaction area, but not released yet.
        //	OVER: mouse cursor is over the interaction area
        //	OUT: mouse cursor is outside the interaction area
        switch (status) {
            case cvui::CLICK:	std::cout << "Rectangle was clicked!" << std::endl; break;
            case cvui::DOWN:	cvui::printf(frame, 240, 70, "Mouse is: DOWN"); break;
            case cvui::OVER:	cvui::printf(frame, 240, 70, "Mouse is: OVER"); break;
            case cvui::OUT:		cvui::printf(frame, 240, 70, "Mouse is: OUT"); break;
        }

        // Show the coordinates of the mouse pointer on the screen
        cvui::printf(frame, 240, 50, "Mouse pointer is at (%d,%d)", cvui::mouse().x, cvui::mouse().y);
        
        // This function must be called *AFTER* all UI components. It does
        // all the behind the scenes magic to handle mouse clicks, etc.
        cvui::update();
        
        // Show everything on the screen
        cv::imshow(WINDOW_NAME, frame);
        
        // Check if ESC key was pressed
        if (cv::waitKey(20) == 27) {
            break;
        }
    }
}

void main_app() {
    std::string WINDOW_NAME = "main_app";
    cv::Mat frame = cv::Mat(300, 600, CV_8UC3);
    bool checked = false;
    bool checked2 = true;
    int count = 0;
    double countFloat = 0.0;
    double trackbarValue = 0.0;
    
    // Init cvui and tell it to create a OpenCV window, i.e. cv::namedWindow(WINDOW_NAME).
    cvui::init(WINDOW_NAME);
    
    while (true) {
        // Fill the frame with a nice color
        frame = cv::Scalar(49, 52, 49);
        
        // Show some pieces of text.
        cvui::text(frame, 50, 30, "Hey there!");
        
        // You can also specify the size of the text and its color
        // using hex 0xRRGGBB CSS-like style.
        cvui::text(frame, 200, 30, "Use hex 0xRRGGBB colors easily", 0.4, 0xff0000);
        
        // Sometimes you want to show text that is not that simple, e.g. strings + numbers.
        // You can use cvui::printf for that. It accepts a variable number of parameter, pretty
        // much like printf does.
        cvui::printf(frame, 200, 50, 0.4, 0x00ff00, "Use printf formatting: %d + %.2f = %f", 2, 3.2, 5.2);
        
        // Buttons will return true if they were clicked, which makes
        // handling clicks a breeze.
        if (cvui::button(frame, 50, 60, "Button")) {
            std::cout << "Button clicked" << std::endl;
        }
        
        // If you do not specify the button width/height, the size will be
        // automatically adjusted to properly house the label.
        cvui::button(frame, 200, 70, "Button with large label");
        
        // You can tell the width and height you want
        cvui::button(frame, 410, 70, 15, 15, "x");
        
        // Window components are useful to create HUDs and similars. At the
        // moment, there is no implementation to constraint content within a
        // a window.
        cvui::window(frame, 50, 120, 120, 100, "Window");
        
        // The counter component can be used to alter int variables. Use
        // the 4th parameter of the function to point it to the variable
        // to be changed.
        cvui::counter(frame, 200, 120, &count);
        
        // Counter can be used with doubles too. You can also specify
        // the counter's step (how much it should change
        // its value after each button press), as well as the format
        // used to print the value.
        cvui::counter(frame, 320, 120, &countFloat, 0.1, "%.1f");
        
        // The trackbar component can be used to create scales.
        // It works with all numerical types (including chars).
        cvui::trackbar(frame, 420, 110, 150, &trackbarValue, 0., 50.);
        
        // Checkboxes also accept a pointer to a variable that controls
        // the state of the checkbox (checked or not). cvui::checkbox() will
        // automatically update the value of the boolean after all
        // interactions, but you can also change it by yourself. Just
        // do "checked = true" somewhere and the checkbox will change
        // its appearance.
        cvui::checkbox(frame, 200, 160, "Checkbox", &checked);
        cvui::checkbox(frame, 200, 190, "A checked checkbox", &checked2);
        
        // Display the lib version at the bottom of the screen
        cvui::printf(frame, frame.cols - 80, frame.rows - 20, 0.4, 0xCECECE, "cvui v.%s", cvui::VERSION);
        
        // This function must be called *AFTER* all UI components. It does
        // all the behind the scenes magic to handle mouse clicks, etc.
        cvui::update();
        
        // Show everything on the screen
        cv::imshow(WINDOW_NAME, frame);
        
        // Check if ESC key was pressed
        if (cv::waitKey(20) == 27) {
            break;
        }
    }
}

void mouse() {
    std::string WINDOW_NAME = "mouse";
    cv::Mat frame = cv::Mat(300, 600, CV_8UC3);
    
    // Init cvui and tell it to create a OpenCV window, i.e. cv::namedWindow(WINDOW_NAME).
    cvui::init(WINDOW_NAME);
    
    // Rectangle to be rendered according to mouse interactions.
    cv::Rect rectangle(0, 0, 0, 0);
    
    while (true) {
        // Fill the frame with a nice color
        frame = cv::Scalar(49, 52, 49);
        
        // Show the coordinates of the mouse pointer on the screen
        cvui::text(frame, 10, 30, "Click (any) mouse button and drag the pointer around to select an area.");
        cvui::printf(frame, 10, 50, "Mouse pointer is at (%d,%d)", cvui::mouse().x, cvui::mouse().y);
        
        // The function "bool cvui::mouse(int query)" allows you to query the mouse for events.
        // E.g. cvui::mouse(cvui::DOWN)
        //
        // Available queries:
        //	- cvui::DOWN: any mouse button was pressed. cvui::mouse() returns true for a single frame only.
        //	- cvui::UP: any mouse button was released. cvui::mouse() returns true for a single frame only.
        //	- cvui::CLICK: any mouse button was clicked (went down then up, no matter the amount of frames in between). cvui::mouse() returns true for a single frame only.
        //	- cvui::IS_DOWN: any mouse button is currently pressed. cvui::mouse() returns true for as long as the button is down/pressed.
        
        // Did any mouse button go down?
        if (cvui::mouse(cvui::DOWN)) {
            // Position the rectangle at the mouse pointer.
            rectangle.x = cvui::mouse().x;
            rectangle.y = cvui::mouse().y;
        }
        
        // Is any mouse button down (pressed)?
        if (cvui::mouse(cvui::IS_DOWN)) {
            // Adjust rectangle dimensions according to mouse pointer
            rectangle.width = cvui::mouse().x - rectangle.x;
            rectangle.height = cvui::mouse().y - rectangle.y;
            
            // Show the rectangle coordinates and size
            cvui::printf(frame, rectangle.x + 5, rectangle.y + 5, 0.3, 0xff0000, "(%d,%d)", rectangle.x, rectangle.y);
            cvui::printf(frame, cvui::mouse().x + 5, cvui::mouse().y + 5, 0.3, 0xff0000, "w:%d, h:%d", rectangle.width, rectangle.height);
        }
        
        // Did any mouse button go up?
        if (cvui::mouse(cvui::UP)) {
            // Hide the rectangle
            rectangle.x = 0;
            rectangle.y = 0;
            rectangle.width = 0;
            rectangle.height = 0;
        }
        
        // Was the mouse clicked (any button went down then up)?
        if (cvui::mouse(cvui::CLICK)) {
            cvui::text(frame, 10, 70, "Mouse was clicked!");
        }
        
        // Render the rectangle
        cvui::rect(frame, rectangle.x, rectangle.y, rectangle.width, rectangle.height, 0xff0000);
        
        // This function must be called *AFTER* all UI components. It does
        // all the behind the scenes magic to handle mouse clicks, etc.
        cvui::update();
        
        // Show everything on the screen
        cv::imshow(WINDOW_NAME, frame);
        
        // Check if ESC key was pressed
        if (cv::waitKey(20) == 27) {
            break;
        }
    }
}

void mouse_complex() {
    std::string WINDOW_NAME = "mouse_complex";
    std::string ROI_WINDOW = "ROI";
    cv::Mat lena = cv::imread("./src/cvui/cvui/example/data/lena.jpg");
    cv::Mat frame = lena.clone();
    cv::Point anchor;
    cv::Rect roi(0, 0, 0, 0);
    bool working = false;
    
    // Init cvui and tell it to create a OpenCV window, i.e. cv::namedWindow(WINDOW_NAME).
    cvui::init(WINDOW_NAME);
    
    while (true) {
        // Fill the frame with Lena's image
        lena.copyTo(frame);
        
        // Show the coordinates of the mouse pointer on the screen
        cvui::text(frame, 10, 10, "Click (any) mouse button and drag the pointer around to select a ROI.");
        
        // The function "bool cvui::mouse(int query)" allows you to query the mouse for events.
        // E.g. cvui::mouse(cvui::DOWN)
        //
        // Available queries:
        //	- cvui::DOWN: any mouse button was pressed. cvui::mouse() returns true for single frame only.
        //	- cvui::UP: any mouse button was released. cvui::mouse() returns true for single frame only.
        //	- cvui::CLICK: any mouse button was clicked (went down then up, no matter the amount of frames in between). cvui::mouse() returns true for single frame only.
        //	- cvui::IS_DOWN: any mouse button is currently pressed. cvui::mouse() returns true for as long as the button is down/pressed.
        
        // Did any mouse button go down?
        if (cvui::mouse(cvui::DOWN)) {
            // Position the anchor at the mouse pointer.
            anchor.x = cvui::mouse().x;
            anchor.y = cvui::mouse().y;
            
            // Inform we are working, so the ROI window is not updated every frame
            working = true;
        }
        
        // Is any mouse button down (pressed)?
        if (cvui::mouse(cvui::IS_DOWN)) {
            // Adjust roi dimensions according to mouse pointer
            int width = cvui::mouse().x - anchor.x;
            int height = cvui::mouse().y - anchor.y;
            
            roi.x = width < 0 ? anchor.x + width : anchor.x;
            roi.y = height < 0 ? anchor.y + height : anchor.y;
            roi.width = std::abs(width);
            roi.height = std::abs(height);
            
            // Show the roi coordinates and size
            cvui::printf(frame, roi.x + 5, roi.y + 5, 0.3, 0xff0000, "(%d,%d)", roi.x, roi.y);
            cvui::printf(frame, cvui::mouse().x + 5, cvui::mouse().y + 5, 0.3, 0xff0000, "w:%d, h:%d", roi.width, roi.height);
        }
        
        // Was the mouse clicked (any button went down then up)?
        if (cvui::mouse(cvui::UP)) {
            // We are done working with the ROI.
            working = false;
        }
        
        // Ensure ROI is within bounds
        roi.x = roi.x < 0 ? 0 : roi.x;
        roi.y = roi.y < 0 ? 0 : roi.y;
        roi.width = roi.x + roi.width > lena.cols ? roi.width + lena.cols - (roi.x + roi.width) : roi.width;
        roi.height = roi.y + roi.height > lena.rows ? roi.height + lena.rows - (roi.y + roi.height) : roi.height;
        
        // Render the roi
        cvui::rect(frame, roi.x, roi.y, roi.width, roi.height, 0xff0000);
        
        // This function must be called *AFTER* all UI components. It does
        // all the behind the scenes magic to handle mouse clicks, etc.
        cvui::update();
        
        // Show everything on the screen
        cv::imshow(WINDOW_NAME, frame);
        
        // If the ROI is valid, show it.
        if (roi.area() > 0 && !working) {
            cv::imshow(ROI_WINDOW, lena(roi));
        }
        
        // Check if ESC key was pressed
        if (cv::waitKey(20) == 27) {
            break;
        }
    }
}

void mouse_complex_buttons() {
    std::string WINDOW_NAME = "mouse_complex_buttons";
    cv::Mat lena = cv::imread("./src/cvui/cvui/example/data/lena.jpg");
	cv::Mat frame = lena.clone();
	cv::Point anchors[3]; // one anchor for each mouse button
	cv::Rect rois[3]; // one ROI for each mouse button
	unsigned int colors[] = { 0xff0000, 0x00ff00, 0x0000ff };

	// Init cvui and tell it to create a OpenCV window, i.e. cv::namedWindow(WINDOW_NAME).
	cvui::init(WINDOW_NAME);

	while (true) {
		// Fill the frame with Lena's image
		lena.copyTo(frame);

		// Show the coordinates of the mouse pointer on the screen
		cvui::text(frame, 10, 10, "Click (any) mouse button then drag the pointer around to select a ROI.");
		cvui::text(frame, 10, 25, "Use different mouse buttons (right, middle and left) to select different ROIs.");

		// Iterate all mouse buttons (left, middle  and right button).
		for (int button = cvui::LEFT_BUTTON; button <= cvui::RIGHT_BUTTON; button++) {
			// Get the anchor, ROI and color associated with the mouse button
			cv::Point& anchor = anchors[button];
			cv::Rect& roi = rois[button];
			unsigned int color = colors[button];

			// The function "bool cvui::mouse(int button, int query)" allows you to query a particular mouse button for events.
			// E.g. cvui::mouse(cvui::RIGHT_BUTTON, cvui::DOWN)
			//
			// Available queries:
			//	- cvui::DOWN: mouse button was pressed. cvui::mouse() returns true for single frame only.
			//	- cvui::UP: mouse button was released. cvui::mouse() returns true for single frame only.
			//	- cvui::CLICK: mouse button was clicked (went down then up, no matter the amount of frames in between). cvui::mouse() returns true for single frame only.
			//	- cvui::IS_DOWN: mouse button is currently pressed. cvui::mouse() returns true for as long as the button is down/pressed.

			// Did the mouse button go down?
			if (cvui::mouse(button, cvui::DOWN)) {
				// Position the anchor at the mouse pointer.
				anchor.x = cvui::mouse().x;
				anchor.y = cvui::mouse().y;
			}

			// Is any mouse button down (pressed)?
			if (cvui::mouse(button, cvui::IS_DOWN)) {
				// Adjust roi dimensions according to mouse pointer
				int width = cvui::mouse().x - anchor.x;
				int height = cvui::mouse().y - anchor.y;

				roi.x = width < 0 ? anchor.x + width : anchor.x;
				roi.y = height < 0 ? anchor.y + height : anchor.y;
				roi.width = std::abs(width);
				roi.height = std::abs(height);

				// Show the roi coordinates and size
				cvui::printf(frame, roi.x + 5, roi.y + 5, 0.3, color, "(%d,%d)", roi.x, roi.y);
				cvui::printf(frame, cvui::mouse().x + 5, cvui::mouse().y + 5, 0.3, color, "w:%d, h:%d", roi.width, roi.height);
			}

			// Ensure ROI is within bounds
			roi.x = roi.x < 0 ? 0 : roi.x;
			roi.y = roi.y < 0 ? 0 : roi.y;
			roi.width = roi.x + roi.width > lena.cols ? roi.width + lena.cols - (roi.x + roi.width) : roi.width;
			roi.height = roi.y + roi.height > lena.rows ? roi.height + lena.rows - (roi.y + roi.height) : roi.height;

			// If the ROI is valid, render it in the frame and show in a window.
			if (roi.area() > 0) {
				cvui::rect(frame, roi.x, roi.y, roi.width, roi.height, color);
				cvui::printf(frame, roi.x + 5, roi.y - 10, 0.3, color, "ROI %d", button);

				cv::imshow("ROI button" + std::to_string(button), lena(roi));
			}
		}

		// This function must be called *AFTER* all UI components. It does
		// all the behind the scenes magic to handle mouse clicks, etc.
		cvui::update();

		// Show everything on the screen
		cv::imshow(WINDOW_NAME, frame);


		// Check if ESC key was pressed
		if (cv::waitKey(20) == 27) {
			break;
		}
	}
}

void multiple_windows() {
	// Init cvui. If you don't tell otherwise, cvui will create the required OpenCV
	// windows based on the list of names you provided.
    std::string WINDOW1_NAME = "window1";
    std::string WINDOW2_NAME = "window2";
    std::string WINDOW3_NAME = "window3";
    std::string WINDOW4_NAME = "window4";
	const cv::String windows[] = { WINDOW1_NAME, WINDOW2_NAME, WINDOW3_NAME, WINDOW4_NAME };
	cvui::init(windows, 4);

	while (true) {
		// The functions below will update a window and show them using cv::imshow().
		// In that case, you must call the pair cvui::context(NAME)/cvui::update(NAME)
		// to render components and update the window.
		window(WINDOW1_NAME);
		window(WINDOW2_NAME);
		window(WINDOW3_NAME);
		
		// The function below will do the same as the funcitons above, however it will
		// use cvui::imshow() (cvui's version of cv::imshow()), which will automatically
		// call cvui::update() for us.
		compact(WINDOW4_NAME);

		// Check if ESC key was pressed
		if (cv::waitKey(20) == 27) {
			break;
		}
	}
}

void multiple_windows_complex() {
    std::string WINDOW1_NAME = "window1";
    std::string WINDOW2_NAME = "window2";
	// We have one mat for each window.
	cv::Mat frame1 = cv::Mat(600, 800, CV_8UC3), frame2 = cv::Mat(600, 800, CV_8UC3);
	
	// Create variables used by some components
	std::vector<double> window1_values;
	std::vector<double> window2_values;
	bool window1_checked = false, window1_checked2 = false;
	bool window2_checked = false, window2_checked2 = false;
	double window1_value = 1.0, window1_value2 = 1.0, window1_value3 = 1.0;
	double window2_value = 1.0, window2_value2 = 1.0, window2_value3 = 1.0;

	cv::Mat img = cv::imread("./src/cvui/cvui/example/data/lena-face.jpg", cv::IMREAD_COLOR);
	cv::Mat imgRed = cv::imread("./src/cvui/cvui/example/data/lena-face-red.jpg", cv::IMREAD_COLOR);
	cv::Mat imgGray = cv::imread("./src/cvui/cvui/example/data/lena-face-gray.jpg", cv::IMREAD_COLOR);

	int padding = 10;

	// Fill the vector with a few random values
	for (std::vector<double>::size_type i = 0; i < 20; i++) {
		window1_values.push_back(rand() + 0.);
		window2_values.push_back(rand() + 0.);
	}

	// Start two OpenCV windows
	// cv::namedWindow(WINDOW1_NAME);
	// cv::namedWindow(WINDOW2_NAME);
	
	// Init cvui and inform it to use the first window as the default one.
	// cvui::init() will automatically watch the informed window.
	cvui::init(WINDOW1_NAME);

	// Tell cvui to keep track of mouse events in window2 as well.
	cvui::watch(WINDOW2_NAME);

	while (true) {
		// Inform cvui that all subsequent component calls and events are related to window 1.
		cvui::context(WINDOW1_NAME);

		// Fill the frame with a nice color
		frame1 = cv::Scalar(49, 52, 49);

		cvui::beginRow(frame1, 10, 20, 100, 50);
			cvui::text("This is ");
			cvui::printf("a row");
			cvui::checkbox("checkbox", &window1_checked);
			cvui::window(80, 80, "window");
			cvui::rect(50, 50, 0x00ff00, 0xff0000);
			cvui::sparkline(window1_values, 50, 50);
			cvui::counter(&window1_value);
			cvui::button(100, 30, "Fixed");
			cvui::image(img);
			cvui::button(img, imgGray, imgRed);
		cvui::endRow();

		padding = 50;
		cvui::beginRow(frame1, 10, 150, 100, 50, padding);
			cvui::text("This is ");
			cvui::printf("another row");
			cvui::checkbox("checkbox", &window1_checked2);
			cvui::window(80, 80, "window");
			cvui::button(100, 30, "Fixed");
			cvui::printf("with 50px padding.");
		cvui::endRow();

		cvui::beginRow(frame1, 10, 250, 100, 50);
			cvui::text("This is ");
			cvui::printf("another row with a trackbar ");
			cvui::trackbar(150, &window1_value2, 0., 5.);
			cvui::printf(" and a button ");
			cvui::button(100, 30, "button");
		cvui::endRow();

		cvui::beginColumn(frame1, 50, 330, 100, 200);
			cvui::text("Column 1 (no padding)");
			cvui::button("button1");
			cvui::button("button2");
			cvui::text("End of column 1");
		cvui::endColumn();

		padding = 10;
		cvui::beginColumn(frame1, 300, 330, 100, 200, padding);
			cvui::text("Column 2 (padding = 10)");
			cvui::button("button1");
			cvui::button("button2");
			cvui::trackbar(150, &window1_value3, 0., 5., 1, "%3.2Lf", cvui::TRACKBAR_DISCRETE, 0.25);
			cvui::text("End of column 2");
		cvui::endColumn();

		cvui::beginColumn(frame1, 550, 330, 100, 200);
			cvui::text("Column 3 (use space)");
			cvui::space(5);
			cvui::button("button1 5px below");
			cvui::space(50);
			cvui::text("Text 50px below");
			cvui::space(20);
			cvui::button("Button 20px below");
			cvui::space(40);
			cvui::text("End of column 2 (40px below)");
		cvui::endColumn();
		
		// Update all components of window1, e.g. mouse clicks, and show it.
		cvui::update(WINDOW1_NAME);
		cv::imshow(WINDOW1_NAME, frame1);

		// From this point on, we are going to render the second window. We need to inform cvui
		// that all updates and components from now on are connected to window 2.
		// We do that by calling cvui::context().
		cvui::context(WINDOW2_NAME);
		frame2 = cv::Scalar(49, 52, 49);
		
		cvui::beginRow(frame2, 10, 20, 100, 50);
			cvui::text("This is ");
			cvui::printf("a row");
			cvui::checkbox("checkbox", &window2_checked);
			cvui::window(80, 80, "window");
			cvui::rect(50, 50, 0x00ff00, 0xff0000);
			cvui::sparkline(window2_values, 50, 50);
			cvui::counter(&window2_value);
			cvui::button(100, 30, "Fixed");
			cvui::image(img);
			cvui::button(img, imgGray, imgRed);
		cvui::endRow();

		padding = 50;
		cvui::beginRow(frame2, 10, 150, 100, 50, padding);
			cvui::text("This is ");
			cvui::printf("another row");
			cvui::checkbox("checkbox", &window2_checked2);
			cvui::window(80, 80, "window");
			cvui::button(100, 30, "Fixed");
			cvui::printf("with 50px padding.");
		cvui::endRow();

		// Another row mixing several components 
		cvui::beginRow(frame2, 10, 250, 100, 50);
		cvui::text("This is ");
		cvui::printf("another row with a trackbar ");
		cvui::trackbar(150, &window2_value2, 0., 5.);
		cvui::printf(" and a button ");
		cvui::button(100, 30, "button");
		cvui::endRow();

		cvui::beginColumn(frame2, 50, 330, 100, 200);
			cvui::text("Column 1 (no padding)");
			cvui::button("button1");
			cvui::button("button2");
			cvui::text("End of column 1");
		cvui::endColumn();

		padding = 10;
		cvui::beginColumn(frame2, 300, 330, 100, 200, padding);
			cvui::text("Column 2 (padding = 10)");
			cvui::button("button1");
			cvui::button("button2");
			cvui::trackbar(150, &window2_value3, 0., 5., 1, "%3.2Lf", cvui::TRACKBAR_DISCRETE, 0.25);
			cvui::text("End of column 2");
		cvui::endColumn();

		cvui::beginColumn(frame2, 550, 330, 100, 200);
			cvui::text("Column 3 (use space)");
			cvui::space(5);
			cvui::button("button1 5px below");
			cvui::space(50);
			cvui::text("Text 50px below");
			cvui::space(20);
			cvui::button("Button 20px below");
			cvui::space(40);
			cvui::text("End of column 2 (40px below)");
		cvui::endColumn();

		// Update all components of window2, e.g. mouse clicks, and show it.
		cvui::update(WINDOW2_NAME);
		cv::imshow(WINDOW2_NAME, frame2);

		// Check if ESC key was pressed
		if (cv::waitKey(20) == 27) {
			break;
		}
	}
}

void multiple_windows_complex_dynamic() {

    std::string GUI_WINDOW1_NAME = "window1";
    std::string GUI_WINDOW2_NAME = "window2";
    std::string ERROR_WINDOW_NAME = "Error window";
    	// We have one mat for each window.
	cv::Mat frame1 = cv::Mat(150, 600, CV_8UC3), frame2 = cv::Mat(150, 600, CV_8UC3), error_frame = cv::Mat(100, 300, CV_8UC3);
	
	// Flag to control if we should show an error window.
	bool error = false;

	// Create two OpenCV windows
	// cv::namedWindow(GUI_WINDOW1_NAME);
	// cv::namedWindow(GUI_WINDOW2_NAME);
	
	// Init cvui and inform it to use the first window as the default one.
	// cvui::init() will automatically watch the informed window.
	cvui::init(GUI_WINDOW1_NAME);

	// Tell cvui to keep track of mouse events in window2 as well.
	cvui::watch(GUI_WINDOW2_NAME);

	while (true) {
		// Inform cvui that all subsequent component calls and events are related to window 1.
		cvui::context(GUI_WINDOW1_NAME);

		// Fill the frame with a nice color
		frame1 = cv::Scalar(49, 52, 49);

		cvui::beginColumn(frame1, 50, 20, -1, -1, 10);
			cvui::text("[Win1] Use the buttons below to control the error window");
			
			if (cvui::button("Close")) {
				closeWindow(ERROR_WINDOW_NAME);
			}

			// If the button is clicked, we open the error window.
			// The content and rendering of such error window will be performed
			// after we handled all other windows.
			if (cvui::button("Open")) {
				error = true;
				openWindow(ERROR_WINDOW_NAME);
			}
		cvui::endColumn();

		// Update all components of window1, e.g. mouse clicks, and show it.
		cvui::update(GUI_WINDOW1_NAME);
		cv::imshow(GUI_WINDOW1_NAME, frame1);

		// From this point on, we are going to render the second window. We need to inform cvui
		// that all updates and components from now on are connected to window 2.
		// We do that by calling cvui::context().
		cvui::context(GUI_WINDOW2_NAME);
		frame2 = cv::Scalar(49, 52, 49);
		
		cvui::beginColumn(frame2, 50, 20, -1, -1, 10);
			cvui::text("[Win2] Use the buttons below to control the error window");

			if (cvui::button("Close")) {
				closeWindow(ERROR_WINDOW_NAME);
			}

			// If the button is clicked, we open the error window.
			// The content and rendering of such error window will be performed
			// after we handled all other windows.
			if (cvui::button("Open")) {
				openWindow(ERROR_WINDOW_NAME);
				error = true;
			}
		cvui::endColumn();

		// Update all components of window2, e.g. mouse clicks, and show it.
		cvui::update(GUI_WINDOW2_NAME);
		cv::imshow(GUI_WINDOW2_NAME, frame2);

		// Handle the content and rendering of the error window,
		// if we have un active error and the window is actually open.
		if (error && isWindowOpen(ERROR_WINDOW_NAME)) {
			// Inform cvui that all subsequent component calls and events are
			// related to the error window from now on
			cvui::context(ERROR_WINDOW_NAME);

			// Fill the error window if a vibrant color
			error_frame = cv::Scalar(10, 10, 49);

			cvui::text(error_frame, 70, 20, "This is an error message", 0.4, 0xff0000);

			if (cvui::button(error_frame, 110, 40, "Close")) {
				error = false;
			}

			if (error) {
				// We still have an active error.
				// Update all components of the error window, e.g. mouse clicks, and show it.
				cvui::update(ERROR_WINDOW_NAME);
				cv::imshow(ERROR_WINDOW_NAME, error_frame);
			} else {
				// No more active error. Let's close the error window.
				closeWindow(ERROR_WINDOW_NAME);
			}
		}

		// Check if ESC key was pressed
		if (cv::waitKey(20) == 27) {
			break;
		}
	}
}

void multiple_windows_complex_mouse() {
    std::string WINDOW1_NAME = "window1";
    std::string WINDOW2_NAME = "window2";
    std::string WINDOW3_NAME = "window3";
    	// We have one cv::Mat for each window.
	cv::Mat frame1 = cv::Mat(200, 500, CV_8UC3), frame2 = cv::Mat(200, 500, CV_8UC3), frame3 = cv::Mat(200, 500, CV_8UC3);

	// Init cvui, instructing it to create 3 OpenCV windows.
	const cv::String windows[] = { WINDOW1_NAME, WINDOW2_NAME, WINDOW3_NAME };
	cvui::init(windows, 3);

	while (true) {
		// clear all frames
		frame1 = cv::Scalar(49, 52, 49);
		frame2 = cv::Scalar(49, 52, 49);
		frame3 = cv::Scalar(49, 52, 49);

		// Inform cvui that all subsequent component calls and events are related to window 1.
		// We do that by calling cvui::context().
		cvui::context(WINDOW1_NAME);
		cvui::printf(frame1, 10, 10, "In window1, mouse is at: %d,%d (obtained from window name)", cvui::mouse(WINDOW1_NAME).x, cvui::mouse(WINDOW1_NAME).y);
		if (cvui::mouse(WINDOW1_NAME, cvui::LEFT_BUTTON, cvui::IS_DOWN)) {
			cvui::printf(frame1, 10, 30, "In window1, mouse LEFT_BUTTON is DOWN");
		}
		cvui::imshow(WINDOW1_NAME, frame1);

		// From this point on, we are going to render the second window. We need to inform cvui
		// that all updates and components from now on are connected to window 2.
		cvui::context(WINDOW2_NAME);
		cvui::printf(frame2, 10, 10, "In window2, mouse is at: %d,%d (obtained from context)", cvui::mouse().x, cvui::mouse().y);
		if (cvui::mouse(cvui::LEFT_BUTTON, cvui::IS_DOWN)) {
			cvui::printf(frame2, 10, 30, "In window2, mouse LEFT_BUTTON is DOWN");
		}
		cvui::imshow(WINDOW2_NAME, frame2);

		// Finally we are going to render the thrid window. Again we need to inform cvui
		// that all updates and components from now on are connected to window 3.
		cvui::context(WINDOW3_NAME);
		cvui::printf(frame3, 10, 10, "In window1, mouse is at: %d,%d", cvui::mouse(WINDOW1_NAME).x, cvui::mouse(WINDOW1_NAME).y);
		cvui::printf(frame3, 10, 30, "In window2, mouse is at: %d,%d", cvui::mouse(WINDOW2_NAME).x, cvui::mouse(WINDOW2_NAME).y);
		cvui::printf(frame3, 10, 50, "In window3, mouse is at: %d,%d", cvui::mouse(WINDOW3_NAME).x, cvui::mouse(WINDOW3_NAME).y);
		if (cvui::mouse(WINDOW1_NAME, cvui::LEFT_BUTTON, cvui::IS_DOWN)) {
			cvui::printf(frame3, 10, 90, "window1: LEFT_BUTTON is DOWN");
		}
		if (cvui::mouse(WINDOW2_NAME, cvui::LEFT_BUTTON, cvui::IS_DOWN)) {
			cvui::printf(frame3, 10, 110, "window2: LEFT_BUTTON is DOWN");
		}
		if (cvui::mouse(WINDOW3_NAME, cvui::LEFT_BUTTON, cvui::IS_DOWN)) {
			cvui::printf(frame3, 10, 130, "window3: LEFT_BUTTON is DOWN");
		}
		cvui::imshow(WINDOW3_NAME, frame3);

		// Check if ESC key was pressed
		if (cv::waitKey(20) == 27) {
			break;
		}
	}
}

void nested_rows_columns() {
    std::string WINDOW_NAME = "nested_rows_columns";
    cv::Mat frame = cv::Mat(600, 800, CV_8UC3);
	std::vector<double> values;
	bool checked = false;
	double value = 1.0;

	// Fill the vector with a few random values
	for (std::vector<double>::size_type i = 0; i < 20; i++) {
		values.push_back(rand() + 0.);
	}

	// Init cvui and tell it to create a OpenCV window, i.e. cv::namedWindow(WINDOW_NAME).
	cvui::init(WINDOW_NAME);

	while (true) {
		// Fill the frame with a nice color
		frame = cv::Scalar(49, 52, 49);

		// Define a row at position (10, 50) with width 100 and height 150.
		cvui::beginRow(frame, 10, 50, 100, 150);
			// The components below will be placed one beside the other.
			cvui::text("Row starts");
			cvui::button("here");

			// When a column or row is nested within another, it behaves like
			// an ordinary component with the specified size. In this case,
			// let's create a column with width 100 and height 50. The
			// next component added will behave like it was added after
			// a component with width 100 and heigth 150.
			cvui::beginColumn(100, 150);
				cvui::text("Column 1");
				cvui::button("button1");
				cvui::button("button2");
				cvui::button("button3");
				cvui::text("End of column 1");
			cvui::endColumn();

			// Add two pieces of text
			cvui::text("Hi again,");
			cvui::text("its me!");

			// Start a new column
			cvui::beginColumn(100, 50);
				cvui::text("Column 2");
				cvui::button("button1");
				cvui::button("button2");
				cvui::button("button3");
				cvui::space();
				cvui::text("Another text");
				cvui::space(40);
				cvui::text("End of column 2");
			cvui::endColumn();

			// Add more text
			cvui::text("this is the ");
			cvui::text("end of the row!");
		cvui::endRow();

		// Here is another nested row/column
		cvui::beginRow(frame, 50, 300, 100, 150);
			// If you don't want to calculate the size of any row/column WITHIN
			// a begin*()/end*() block, just use negative width/height when
			// calling beginRow() or beginColumn() (or don't provide width/height at all!)

			// For instance, the following column will have its width/height
			// automatically adjusted according to its content.
			cvui::beginColumn();
				cvui::text("Column 1");
				cvui::button("button with very large label");
				cvui::text("End of column 1");
			cvui::endColumn();

			// Add two pieces of text
			cvui::text("Hi again,");
			cvui::text("its me!");

			// Start a new column
			cvui::beginColumn();
				cvui::text("Column 2");
				cvui::button("btn");
				cvui::space();
				cvui::text("text");
				cvui::button("btn2");
				cvui::text("text2");
				if (cvui::button("&Quit")) {
          			break;
        		}
			cvui::endColumn();

			// Add more text
			cvui::text("this is the ");
			cvui::text("end of the row!");
		cvui::endRow();
		
		// This function must be called *AFTER* all UI components. It does
		// all the behind the scenes magic to handle mouse clicks, etc.
		cvui::update();

		// Show everything on the screen
		cv::imshow(WINDOW_NAME, frame);

		// Check if ESC key was pressed
		if (cv::waitKey(20) == 27) {
			break;
		}
	}
}

void on_image() {
    std::string WINDOW_NAME = "on_image";
    cv::Mat lena = cv::imread("./src/cvui/cvui/example/data/lena.jpg");
	cv::Mat frame = lena.clone();
	cv::Mat doubleBuffer = frame.clone();
	int trackbarWidth = 130;

	// Init cvui and tell it to use a value of 20 for cv::waitKey()
	// because we want to enable keyboard shortcut for
	// all components, e.g. button with label "&Quit".
	// If cvui has a value for waitKey, it will call
	// waitKey() automatically for us within cvui::update().
	cvui::init(WINDOW_NAME, 20);

	while (true) {
		doubleBuffer.copyTo(frame);

		// Exit the application if the quit button was pressed.
		// It can be pressed because of a mouse click or because 
		// the user pressed the "q" key on the keyboard, which is
		// marked as a shortcut in the button label ("&Quit").
		if (cvui::button(frame, frame.cols - 100, frame.rows - 30, "&Quit")) {
			break;
		}

		// RGB HUD
		cvui::window(frame, 20, 50, 180, 240, "RGB adjust");

		// Within the cvui::beginColumns() and cvui::endColumn(),
		// all elements will be automatically positioned by cvui.
		// In a columns, all added elements are vertically placed,
		// one under the other (from top to bottom).
		//
		// Notice that all component calls within the begin/end block
		// below DO NOT have (x,y) coordinates.
		//
		// Let's create a row at position (35,80) with automatic
		// width and height, and a padding of 10
		cvui::beginColumn(frame, 35, 80, -1, -1, 10);
			static double rgb[3] {1., 1., 1};
			bool rgbModified = false;

			// Trackbar accept a pointer to a variable that controls their value
			// They return true upon edition
			if (cvui::trackbar(trackbarWidth, &rgb[0], (double)0., (double)2., 2, "%3.02Lf")) {
				rgbModified = true;
			}
			if (cvui::trackbar(trackbarWidth, &rgb[1], (double)0., (double)2., 2, "%3.02Lf")) {
				rgbModified = true;
			}
			if (cvui::trackbar(trackbarWidth, &rgb[2], (double)0., (double)2., 2, "%3.02Lf")) {
				rgbModified = true;
			}
			
			cvui::space(2);
			cvui::printf(0.35, 0xcccccc, "   RGB: %3.02lf,%3.02lf,%3.02lf", rgb[0], rgb[1], rgb[2]);

			if (rgbModified) {
				std::vector<cv::Mat> channels(3);
				cv::split(lena, channels);
				for (int c = 0; c < 3; c++) {
					channels[c] = channels[c] * rgb[c];
				}
				cv::merge(channels, doubleBuffer);
			}
		cvui::endColumn();

		// HSV
		cvui::window(frame, lena.cols - 200, 50, 180, 240, "HSV adjust");
		cvui::beginColumn(frame, lena.cols - 180, 80, -1, -1, 10);
			static double hsv[3] {1., 1., 1};
			bool hsvModified = false;
			
			if (cvui::trackbar(trackbarWidth, &hsv[0], (double)0., (double)2., 2, "%3.02Lf")) {
				hsvModified = true;
			}
			if (cvui::trackbar(trackbarWidth, &hsv[1], (double)0., (double)2., 2, "%3.02Lf")) {
				hsvModified = true;
			}
			if (cvui::trackbar(trackbarWidth, &hsv[2], (double)0., (double)2., 2, "%3.02Lf")) {
				hsvModified = true;
			}

			cvui::space(2);
			cvui::printf(0.35, 0xcccccc, "   HSV: %3.02lf,%3.02lf,%3.02lf", hsv[0], hsv[1], hsv[2]);

			if (hsvModified) {
				cv::Mat hsvMat;
				cv::cvtColor(lena, hsvMat, cv::COLOR_BGR2HSV);
				std::vector<cv::Mat> channels(3);
				cv::split(hsvMat, channels);

				for (int c = 0; c < 3; c++) {
					channels[c] = channels[c] * hsv[c];
				}

				cv::merge(channels, hsvMat);
				cv::cvtColor(hsvMat, doubleBuffer, cv::COLOR_HSV2BGR);
			}
		cvui::endColumn();

		// Display the lib version at the bottom of the screen
		cvui::printf(frame, frame.cols - 300, frame.rows - 20, 0.4, 0xCECECE, "cvui v.%s", cvui::VERSION);

		// This function must be called *AFTER* all UI components. It does
		// all the behind the scenes magic to handle mouse clicks, etc.
		//
		// Since cvui::init() received a param regarding waitKey,
		// there is no need to call cv::waitKey() anymore. cvui::update()
		// will do it automatically.
		cvui::update();

		// Show everything on the screen
		cv::imshow(WINDOW_NAME, frame);
	}
}

void sparkline() {
    std::string WINDOW_NAME = "sparkline";
    	cv::Mat frame = cv::Mat(600, 800, CV_8UC3);

	// Init cvui and tell it to create a OpenCV window, i.e. cv::namedWindow(WINDOW_NAME).
	cvui::init(WINDOW_NAME);

	// Load some data points from a file
	std::vector<double> points = load("./src/cvui/cvui/example/data/sparkline.csv");
	
	// Create less populated sets
	std::vector<double> few_points;
	std::vector<double> no_points;
	
	for (std::vector<double>::size_type i = 0; i < 30; i++) {
		few_points.push_back(rand() + 0.);
	}

	while (true) {
		// Fill the frame with a nice color
		frame = cv::Scalar(49, 52, 49);

		// Add 3 sparklines that are displaying the same data, but with
		// different width/height/colors.
		cvui::sparkline(frame, points, 0, 0, 800, 200);
		cvui::sparkline(frame, points, 0, 200, 800, 100, 0xff0000);
		cvui::sparkline(frame, points, 0, 300, 400, 100, 0x0000ff);
		
		// Add a sparkline with few points
		cvui::sparkline(frame, few_points, 10, 400, 790, 80, 0xff00ff);

		// Add a sparkline that has no data. In that case, cvui will
		// render it with a visual warning.
		cvui::sparkline(frame, no_points, 10, 500, 750, 100, 0x0000ff);
		
		// This function must be called *AFTER* all UI components. It does
		// all the behind the scenes magic to handle mouse clicks, etc.
		cvui::update();

		// Show everything on the screen
		cv::imshow(WINDOW_NAME, frame);

		// Check if ESC key was pressed
		if (cv::waitKey(20) == 27) {
			break;
		}
	}
}
}