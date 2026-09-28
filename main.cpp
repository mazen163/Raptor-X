#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>
#include "Image_Class.h"

using namespace std;

// Prototyping Helpers
string getValidFilename();
void displayMenu();

// Prototyping 8 Filters for Part 1
void applyGrayscale(Image& img);
void applyBlackAndWhite(Image& img);
void applyInvert(Image& img);
void applyMerge(Image& img);
void applyFlip(Image& img);
void applyRotate(Image& img);
void applyDarkenLighten(Image& img);
void applyResize(Image& img);

int main() {
    cout << "=======================================\n";
    cout << "   WELCOME TO BABY PHOTOSHOP (Part 1)  \n";
    cout << "=======================================\n\n";

    string filename = getValidFilename();
    Image currentImage;

    try {
        currentImage.loadNewImage(filename);
        cout << ">> Image loaded successfully: " << filename << "\n";
    } catch (...) {
        cout << ">> Error: Could not load the image file.\n";
        return 1;
    }

    bool running = true;
    while (running) {
        displayMenu();
        int choice;
        cout << "Enter your choice (1-8 or 0 to exit): ";
        cin >> choice;

        switch (choice) {
            case 1:
                applyGrayscale(currentImage);
                cout << ">> Grayscale Filter Applied!\n";
                break;
            case 2:
                applyBlackAndWhite(currentImage);
                cout << ">> Black and White Filter Applied!\n";
                break;
            case 3:
                applyInvert(currentImage);
                cout << ">> Invert Filter Applied!\n";
                break;
            case 4:
                applyMerge(currentImage);
                cout << ">> Merge Filter Applied!\n";
                break;
            case 5:
                applyFlip(currentImage);
                cout << ">> Flip Filter Applied!\n";
                break;
            case 6:
                applyRotate(currentImage);
                cout << ">> Rotate Filter Applied!\n";
                break;
            case 7:
                applyDarkenLighten(currentImage);
                cout << ">> Darken/Lighten Filter Applied!\n";
                break;
            case 8:
                applyResize(currentImage);
                cout << ">> Resize Filter Applied!\n";
                break;
            case 0:
                running = false;
                break;
            default:
                cout << ">> Invalid choice. Try again.\n";
                break;
        }

        if (running && choice >= 1 && choice <= 8) {
            char saveAns;
            cout << "\nDo you want to save current image changes? (y/n): ";
            cin >> saveAns;
            if (saveAns == 'y' || saveAns == 'Y') {
                string saveName;
                cout << "Enter output image filename (e.g., output.jpg, res.png): ";
                cin >> saveName;
                currentImage.saveImage(saveName);
                cout << ">> Saved successfully to " << saveName << "\n";
            }
        }
    }

    char finalSave;
    cout << "\nSave image before exiting? (y/n): ";
    cin >> finalSave;
    if (finalSave == 'y' || finalSave == 'Y') {
        string saveName;
        cout << "Enter final image filename: ";
        cin >> saveName;
        currentImage.saveImage(saveName);
        cout << ">> Saved. Goodbye!\n";
    }

    return 0;
}

string getValidFilename() {
    string name;
    while (true) {
        cout << "Enter input image filename (.jpg, .png, .bmp): ";
        cin >> name;
        if (name.find(".jpg") != string::npos || name.find(".png") != string::npos ||
            name.find(".bmp") != string::npos || name.find(".jpeg") != string::npos) {
            return name;
        }
        cout << ">> Invalid extension! Please enter a valid .jpg, .png, or .bmp image.\n";
    }
}

void displayMenu() {
    cout << "\n-------------------------------------\n";
    cout << "          AVAILABLE FILTERS          \n";
    cout << "-------------------------------------\n";
    cout << "1. Convert to Grayscale\n";
    cout << "2. Convert to Black & White\n";
    cout << "3. Invert Image Colors\n";
    cout << "4. Merge Two Images\n";
    cout << "5. Flip Image (Horizontal / Vertical)\n";
    cout << "6. Rotate Image (90, 180, 270 degrees)\n";
    cout << "7. Darken / Lighten Image (50%)\n";
    cout << "8. Resize Image\n";
    cout << "0. Exit Program\n";
    cout << "-------------------------------------\n";
}

void applyGrayscale(Image& img) {
    for (int i = 0; i < img.width; ++i) {
        for (int j = 0; j < img.height; ++j) {
            unsigned int avg = 0;
            for (int k = 0; k < img.channels; ++k) {
                avg += img(i, j, k);
            }
            avg /= img.channels;
            for (int k = 0; k < img.channels; ++k) {
                img(i, j, k) = avg;
            }
        }
    }
}

void applyBlackAndWhite(Image& img) {
    for (int i = 0; i < img.width; ++i) {
        for (int j = 0; j < img.height; ++j) {
            unsigned int avg = 0;
            for (int k = 0; k < img.channels; ++k) {
                avg += img(i, j, k);
            }
            avg /= img.channels;
            unsigned char bw = (avg >= 128) ? 255 : 0;
            for (int k = 0; k < img.channels; ++k) {
                img(i, j, k) = bw;
            }
        }
    }
}

void applyInvert(Image& img) {
    for (int i = 0; i < img.width; ++i) {
        for (int j = 0; j < img.height; ++j) {
            for (int k = 0; k < img.channels; ++k) {
                img(i, j, k) = 255 - img(i, j, k);
            }
        }
    }
}

void applyMerge(Image& img) {
    string secondFilename = getValidFilename();
    Image img2;
    try {
        img2.loadNewImage(secondFilename);
    } catch (...) {
        cout << ">> Failed to load second image.\n";
        return;
    }

    Image resizedImg2(img.width, img.height);
    for (int i = 0; i < img.width; ++i) {
        for (int j = 0; j < img.height; ++j) {
            int srcX = i * img2.width / img.width;
            int srcY = j * img2.height / img.height;
            for (int k = 0; k < img.channels; ++k) {
                resizedImg2(i, j, k) = img2(srcX, srcY, min(k, img2.channels - 1));
            }
        }
    }

    for (int i = 0; i < img.width; ++i) {
        for (int j = 0; j < img.height; ++j) {
            for (int k = 0; k < img.channels; ++k) {
                img(i, j, k) = (img(i, j, k) + resizedImg2(i, j, k)) / 2;
            }
        }
    }
}

void applyFlip(Image& img) {
    int dir;
    cout << "1. Flip Horizontally\n2. Flip Vertically\nChoice: ";
    cin >> dir;

    if (dir == 1) {
        for (int i = 0; i < img.width / 2; ++i) {
            for (int j = 0; j < img.height; ++j) {
                for (int k = 0; k < img.channels; ++k) {
                    swap(img(i, j, k), img(img.width - 1 - i, j, k));
                }
            }
        }
    } else if (dir == 2) {
        for (int i = 0; i < img.width; ++i) {
            for (int j = 0; j < img.height / 2; ++j) {
                for (int k = 0; k < img.channels; ++k) {
                    swap(img(i, j, k), img(i, img.height - 1 - j, k));
                }
            }
        }
    }
}

void applyRotate(Image& img) {
    int degree;
    cout << "Enter rotation degree (90, 180, 270): ";
    cin >> degree;

    if (degree == 180) {
        Image rotated(img.width, img.height);
        for (int i = 0; i < img.width; ++i) {
            for (int j = 0; j < img.height; ++j) {
                for (int k = 0; k < img.channels; ++k) {
                    rotated(i, j, k) = img(img.width - 1 - i, img.height - 1 - j, k);
                }
            }
        }
        img = rotated;
    } else if (degree == 90) {
        Image rotated(img.height, img.width);
        for (int i = 0; i < img.width; ++i) {
            for (int j = 0; j < img.height; ++j) {
                for (int k = 0; k < img.channels; ++k) {
                    rotated(img.height - 1 - j, i, k) = img(i, j, k);
                }
            }
        }
        img = rotated;
    } else if (degree == 270) {
        Image rotated(img.height, img.width);
        for (int i = 0; i < img.width; ++i) {
            for (int j = 0; j < img.height; ++j) {
                for (int k = 0; k < img.channels; ++k) {
                    rotated(j, img.width - 1 - i, k) = img(i, j, k);
                }
            }
        }
        img = rotated;
    }
}

void applyDarkenLighten(Image& img) {
    int choice;
    cout << "1. Darken (50%)\n2. Lighten (50%)\nChoice: ";
    cin >> choice;
    float factor = (choice == 1) ? 0.5f : 1.5f;

    for (int i = 0; i < img.width; ++i) {
        for (int j = 0; j < img.height; ++j) {
            for (int k = 0; k < img.channels; ++k) {
                int newVal = static_cast<int>(img(i, j, k) * factor);
                img(i, j, k) = min(255, max(0, newVal));
            }
        }
    }
}

void applyResize(Image& img) {
    int newW, newH;
    cout << "Enter new width and new height: ";
    cin >> newW >> newH;

    Image resized(newW, newH);
    for (int i = 0; i < newW; ++i) {
        for (int j = 0; j < newH; ++j) {
            int srcX = i * img.width / newW;
            int srcY = j * img.height / newH;
            for (int k = 0; k < img.channels; ++k) {
                resized(i, j, k) = img(srcX, srcY, k);
            }
        }
    }
    img = resized;
}