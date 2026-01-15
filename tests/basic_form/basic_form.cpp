// basic_form.cpp : Defines the entry point for the application.
//

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "../../include/mgui.h"
#include <sstream>

class SimpleForm : public CFrame {
    CEdit name, email;
    CCheckBox accept_terms;
public:
    void create() {
        CLabel l1, l2;
        CButton ok, clear;
        int text_h = 24, label_h = 12;
        int gap = 4;
        int y = 0;

        CFrame::create("A Simple Form", 700, 450);

        l1.create("Full name:", 10, y, 300, label_h, this); y += label_h + gap;
        name.create(10, y, 300, text_h, this); y += text_h + gap + gap;

        l2.create("E-mail:", 10, y, 300, label_h, this); y += label_h + gap;
        email.create(10, y, 300, text_h, this);
        y += text_h + gap + gap;

        accept_terms.create("Accept terms and conditions", 10, y, 300, label_h, this, (HMENU) 2001);

        accept_terms.setCheck(true);

        y += label_h + gap + gap;

        ok.create("Submit", 10, y, 60, text_h, this, (HMENU) 2002);
        clear.create("Reset", 80, y, 60, text_h, this, (HMENU)2003);
    }
    void onClose() {
        CWindow::stop();
    }
    void onCommand(int id, int type, CWindow* source) {
        if (id == 2001) {
            accept_terms.setCheck(!accept_terms.getCheck());
        }
        else if (id == 2002) {
            std::ostringstream oss;
            std::string name_txt, email_txt;

            name.getText(name_txt);
            email.getText(email_txt);

            oss << "Name: " << name_txt << "\n";
            oss << "Email: " << email_txt << "\n";
            oss << "Terms accepted: " << (accept_terms.getCheck() ? "Yes" : "No");

            messageBox(oss.str().c_str());
        }
        else if (id == 2003) {
            name.setText("");
            email.setText("");
        }
    }
};

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    CWindow::init(hInstance);

    SimpleForm f;

    f.create();
    f.show();

    CWindow::loop();
}

