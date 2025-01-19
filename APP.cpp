#include "ioset.hpp"

// �{�^��ID
#define ID_LIST 2000
#define ID_STORE 2001    
#define ID_SHUTOP 2002
#define ID_SANSHO 2003
#define ID_CMDEXE 2004
#define ID_TIME 2005
#define ID_MSGCREATE 2006
#define ID_SEARCH 2007
#define ID_SEARCHBOX 2008
#define ID_CMDBOX 2009
#define ID_PATH 2010

// ���O
HWND check;
HWND radio0, radio1, radio2, radio3, radio4, radio5;
HWND combo;
HWND text, second, filePath, powerOption, cmdPrompt, dialogIcon, msgTit, msgTex, browserQuery, searchEngine;

IOSet io("");
class APP{
    protected:
        MSG msg;
        WNDCLASS winc;
        HWND textEdit, cmdGroup, dialogGroup, dialogSettings;
        HINSTANCE hInstance;
        HWND appBody; //HINSTANCE�̉��Ɋm��

        static LRESULT CALLBACK WndProc(HWND window , UINT requestMsg , WPARAM wp , LPARAM lp) {
            int wmId = LOWORD(wp);
            int checkbox = 0;
            int msgVal, mbButton, mbIcon ,help;
            int radioval[6] = {0,0,0,0,0,0};
            int pwOptionList = -1;
            int shutsec;
            int engineList = 0;
            int iconList = 0;
            std::string mbTitle, mbText, url, fileData;
            HDC hdc;
            PAINTSTRUCT ps;
            CHAR textVal[32768], secondVal[32768], fileVal[32768], cmdVal[32768], tex[32768], tit[32768], searchVal[32768];

            switch (requestMsg) { //Window����̃��b�Z�[�W�𕪗�
                case WM_CLOSE: /* �~ �{�^���������ꂽ���̑��� */
                    SetWindowText(window,"�I��");
                    msgVal = io.MsgBox("�m�F","�I�����܂����H",4,3,0);
                    switch (msgVal){
                        case IDYES:
                            DestroyWindow(window);
                            PostQuitMessage(0);
                            return 0;

                        default:
                            SetWindowText(window, io.title("").c_str());
                            return 0;
                    }
                    
                case WM_CTLCOLORSTATIC: /*�R���g���[���X�^�e�B�b�N���Ԃ��Ă���ꏊ(�O���[�v�{�b�N�X)�̐ݒ�*/
                    SetBkMode((HDC)wp,TRANSPARENT); //�w�i�𓧖��ɂ����Y��ɂ���
                    if(powerOption ==(HWND)lp){
                        SetTextColor((HDC)wp,RGB(255,0,0));//�e�L�X�g
                    }
                    return (LRESULT)CreateSolidBrush( RGB( 234, 234, 234 ) ); //�w�i�F

                case WM_LBUTTONDOWN: /*���N���b�N�������Ƃ��̑���*/
                    SetWindowText(window,"���N���b�N");
                    break;

                /*���N���b�N�������Ƃ��̑���*/
                case WM_RBUTTONDOWN:
                    SetWindowText(window,"�E�N���b�N");
                    break;

                /*�t�H�[�J�X���O�����Ƃ��̑���*/
                case EN_KILLFOCUS:
                    SetWindowText(window, io.title("").c_str());
                    break;

                /*Window�ɕ`�悷��ꏊ�̐ݒ�*/
                case WM_PAINT:
                    hdc = BeginPaint(window , &ps);

                    /* �e�L�X�g���� */
                    SetBkColor(hdc , RGB(234 , 234 , 234));
                    SetTextColor(hdc , RGB(0 , 0 , 0));
                    TextOut(hdc , 75 , 110 , TEXT("�b") , lstrlen(TEXT("�b")));

                    SetBkColor(hdc , RGB(0 , 0 , 0));
                    SetTextColor(hdc , RGB(0 , 255 , 0));
                    TextOut(hdc , 280 , 75 , std::string(io.pwd() + " >   ").c_str() ,std::string(io.pwd() + " >   ").size());
                    //�e�L�X�g�ǉ�


                    EndPaint(window , &ps);/* �e�L�X�g�����I�� */
                    break;

                case WM_COMMAND:{ /*�{�^���������Ƃ��̑���*/
                    // �I�����ꂽ���𒲂ׂ�
                    switch (wmId){
                        case 1:
                            SetWindowText(window,"�e�L�X�g�����");
                            break;

                        case ID_PATH:
                            SetWindowText(window,"�Q�Ƃ������t�@�C���̃p�X�̓���");
                            break;

                        case ID_SEARCHBOX:
                            SetWindowText(window,"�����{�b�N�X");
                            break;

                        case ID_LIST:
                            SetWindowText(window,"���X�g����I��");
                            break;

                        case ID_CMDBOX:
                            SetWindowText(window,"�R�}���h����");
                            break;

                        case ID_TIME:
                            SetWindowText(window, "���s����܂ł̎���(�b��)�����");
                            break;

                        case ID_SHUTOP:
                            SetWindowText(window,"���s");
                            pwOptionList = SendMessage(combo, CB_GETCURSEL, 0, 0);
                            GetWindowText(second , secondVal , 32768);
                            shutsec = atoi(secondVal);
                            switch(pwOptionList){
                                case 0:
                                    DestroyWindow(window) ;
                                    PostQuitMessage(0);
                                    io.shutdown("logoff",shutsec);
                                    return 0;
                                case 1:
                                    DestroyWindow(window) ;
                                    PostQuitMessage(0);
                                    io.shutdown("shutdown",shutsec);
                                    return 0;
                                case 2:
                                    DestroyWindow(window) ;
                                    PostQuitMessage(0);
                                    io.shutdown("reboot",shutsec);
                                    return 0;
                            }
                            SetWindowText(window,"���s�o���܂���");
                            break;

                        case ID_STORE:
                            SetWindowText(window,"�ύX�̕ۑ�");
                            msgVal = io.MsgBox("�m�F","���e��ۑ����܂����H",4,3,0);
                            switch (msgVal){
                                case IDYES:
                                    GetWindowText(text , textVal , 32768);
                                    GetWindowText(filePath , fileVal , 32768);
                                    io.write_file(std::string(fileVal), std::string(textVal), "w");
                            }
                            break;

                        case ID_SANSHO:
                            SetWindowText(window,"�Q��");
                            GetWindowText(filePath , fileVal , 32768);
                            fileData = io.read_file(std::string(fileVal));
                            SetWindowText(text, fileData.c_str());
                            break;

                        case ID_CMDEXE:
                            SetWindowText(window,"�R�}���h�v�����v�g���s");
                            GetWindowText(cmdPrompt, cmdVal, 32768);
                            io.cmd(std::string(cmdVal));/* ���s */
                            break;

                        case ID_MSGCREATE:
                            SetWindowText(window,"���b�Z�[�W�{�b�N�X����");

                            GetWindowText(msgTex , tex , 32768);
                            GetWindowText(msgTit , tit , 32768);

                            radioval[0] = SendMessage(radio0, BM_GETCHECK, 0, 0);
                            radioval[1] = SendMessage(radio1, BM_GETCHECK, 0, 0);
                            radioval[2] = SendMessage(radio2, BM_GETCHECK, 0, 0);
                            radioval[3] = SendMessage(radio3, BM_GETCHECK, 0, 0);
                            radioval[4] = SendMessage(radio4, BM_GETCHECK, 0, 0);
                            radioval[5] = SendMessage(radio5, BM_GETCHECK, 0, 0);

                            checkbox = SendMessage(check, BM_GETCHECK, 0, 0);
                            iconList = SendMessage(dialogIcon, CB_GETCURSEL, 0, 0);

                            if(radioval[0] == 1)        mbButton = MB_OK;
                            else if(radioval[1] == 1)   mbButton = MB_OKCANCEL;
                            else if(radioval[2] == 1)   mbButton = MB_YESNO;
                            else if(radioval[3] == 1)   mbButton = MB_YESNOCANCEL;
                            else if(radioval[4] == 1)   mbButton = MB_ABORTRETRYIGNORE;
                            else if(radioval[5] == 1)   mbButton = MB_RETRYCANCEL;

                            mbIcon  = iconList;
                            help    = checkbox;
                            mbTitle = std::string(tit);
                            mbText  = std::string(tex);

                            io.MsgBox(mbTitle, mbText, mbButton, mbIcon, help); 
                            break;

                        case ID_SEARCH:
                            SetWindowText(window,"����");
                            engineList = SendMessage(searchEngine, CB_GETCURSEL, 0, 0);
                            switch(engineList){
                                case 1:
                                    url = "youtube.co.jp/";
                                    break;
                                case 2:
                                    url = "bing.com/";
                                    break;
                                case 3:
                                    url = "twitter.com/";
                                    break;
                                default:
                                    url = "google.co.jp/";
                                    break;
                            }
                            GetWindowText(browserQuery , searchVal , 32768);
                            url = "start www."+ url +"search?q=" + std::string(searchVal);
                            io.cmd(url);
                            break;

                        default:
                            SetWindowText(window,io.title("").c_str());
                    }

                }
            }
            return DefWindowProc(window, requestMsg, wp, lp);
        }

        void ui_setting(){
            this->appBody = CreateWindow( /* Window���� */
                "WINDOW",
                TEXT(io.title("").c_str()) ,
                (WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX) | WS_VISIBLE ,
                100 , 10 ,970 , 610 ,
                NULL , NULL , this->hInstance , NULL
            );
            
            /* ���X�g�{�b�N�X */
            searchEngine = CreateWindow(
                "COMBOBOX",
                NULL, 
                WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST , 
                20 , 10 , 150 , 150 , 
                this->appBody , (HMENU)ID_LIST ,this->hInstance, NULL
            );
            // ���X�g�{�b�N�X�̑I����
            SendMessage(searchEngine , CB_ADDSTRING , 0 , (LPARAM)TEXT("Google"));
            SendMessage(searchEngine , CB_ADDSTRING , 0 , (LPARAM)TEXT("YouTube"));
            SendMessage(searchEngine , CB_ADDSTRING , 0 , (LPARAM)TEXT("Bing"));
            SendMessage(searchEngine , CB_ADDSTRING , 0 , (LPARAM)TEXT("Twitter"));


            /* �e�L�X�g�G���A���� */
            browserQuery = CreateWindow(
                "EDIT",
                NULL,
                WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_LEFT | ES_AUTOHSCROLL | WS_HSCROLL,
                180,10,620,40,
                this->appBody,(HMENU)ID_SEARCHBOX,this->hInstance,NULL
            );

            SetWindowText(browserQuery, TEXT("����")); /* �e�L�X�g�G���A�ɕ����\�� */

            /* �{�^������ */
            CreateWindow(
                "BUTTON",
                TEXT("search"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
                820 , 10 , 100 , 30 ,
                this->appBody , (HMENU)ID_SEARCH , this->hInstance , NULL
            );    

            /* �O���[�v�{�b�N�X���� */
            powerOption = CreateWindow(
                "BUTTON" , "�d���I�v�V����" ,
                WS_CHILD | WS_VISIBLE | BS_GROUPBOX ,
                20 , 50 , 240 , 120 ,
                this->appBody , NULL , this->hInstance , NULL
            );


            /* ���X�g�{�b�N�X */
            combo = CreateWindow(
                "COMBOBOX",
                NULL, 
                WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST , 
                40 , 75 , 200 , 200 , 
                this->appBody , (HMENU)ID_LIST ,this->hInstance, NULL
            );
            // ���X�g�{�b�N�X�̑I����
            SendMessage(combo , CB_ADDSTRING , 0 , (LPARAM)TEXT("���O�I�t"));
            SendMessage(combo , CB_ADDSTRING , 0 , (LPARAM)TEXT("�V���b�g�_�E��"));
            SendMessage(combo , CB_ADDSTRING , 0 , (LPARAM)TEXT("�ċN��"));

            /* �e�L�X�g�G���A���� */
            second = CreateWindow(
                "EDIT",
                NULL,
                WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_RIGHT | ES_NUMBER,
                40,110,30,20,
                this->appBody,(HMENU)ID_TIME,this->hInstance,NULL
            );

            /* �e�L�X�g�G���A�ɕ����\�� */
            SetWindowText(second, TEXT("30"));

            /* �{�^������ */
            CreateWindow(
                "BUTTON",
                TEXT("����"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
                140 , 130 , 100 , 30 ,
                this->appBody , (HMENU)ID_SHUTOP , this->hInstance , NULL
            );

            /* �O���[�v�{�b�N�X���� */
            this->textEdit = CreateWindow(
                "BUTTON" , "�e�L�X�g�ҏW" ,
                WS_CHILD | WS_VISIBLE | BS_GROUPBOX ,
                20 , 190 , 500 , 380 ,
                this->appBody , NULL , this->hInstance , NULL
            );

            /* �e�L�X�g�G���A���� */
            filePath = CreateWindow(
                "EDIT",
                NULL,
                WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_LEFT | ES_AUTOHSCROLL | WS_HSCROLL,
                40,215,460,40,
                this->appBody,(HMENU)ID_PATH,this->hInstance,NULL
            );

            /* �e�L�X�g�G���A�ɕ����\�� */
            SetWindowText(filePath, io.pwd().c_str());

            /* �{�^������ */
            CreateWindow(
                "BUTTON",
                TEXT("�Q��"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
                370 , 260 , 100 , 30 ,
                this->appBody , (HMENU)ID_SANSHO , this->hInstance , NULL
            );


            /* �e�L�X�g�G���A���� */
            text = CreateWindow(
                "EDIT",
                NULL,
                WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_LEFT | ES_AUTOVSCROLL | WS_VSCROLL,
                40,310,400,200,
                this->appBody,(HMENU)1,this->hInstance,NULL
            );

            /* �e�L�X�g�G���A�ɕ����\�� */
            SetWindowText(text, TEXT("�e�L�X�g"));


            /* �{�^������ */
            CreateWindow(
                "BUTTON",
                TEXT("�ۑ�"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
                370 , 520 , 100 , 30 ,
                this->appBody , (HMENU)ID_STORE , this->hInstance , NULL
            );

            /* �O���[�v�{�b�N�X���� */
            this->cmdGroup = CreateWindow(
                "BUTTON" , "�R�}���h�v�����v�g" ,
                WS_CHILD | WS_VISIBLE | BS_GROUPBOX ,
                270 , 50 , 670 , 120 ,
                this->appBody , NULL , this->hInstance , NULL
            );

            /* �e�L�X�g�G���A���� */
            cmdPrompt = CreateWindow(
                "EDIT",
                NULL,
                WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_LEFT | ES_AUTOHSCROLL | WS_HSCROLL,
                280 + 8*(std::string(io.pwd() + " >   ").size()), 75, 910-(280 + 8*(std::string(io.pwd() + " >   ").size()) - 4), 40,
                this->appBody,(HMENU)ID_CMDBOX,this->hInstance,NULL
            );

            /* �{�^������ */
            CreateWindow(
                "BUTTON",
                TEXT("���s"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
                720 , 125 , 100 , 30 ,
                this->appBody , (HMENU)ID_CMDEXE , this->hInstance , NULL
            );

            /* �O���[�v�{�b�N�X���� */
            this->dialogGroup = CreateWindow(
                "BUTTON" , "���b�Z�[�W�{�b�N�X�쐬" ,
                WS_CHILD | WS_VISIBLE | BS_GROUPBOX ,
                535 , 190 , 405 , 380 ,
                this->appBody , NULL , this->hInstance , NULL
            );

            /* �O���[�v�{�b�N�X���� */
            this->dialogSettings = CreateWindow(
                "BUTTON" , "�ݒ�" ,
                WS_CHILD | WS_VISIBLE | BS_GROUPBOX  ,
                545 , 213 , 390 , 145 ,
                this->appBody , NULL , this->hInstance , NULL
            );

            /* ���X�g�{�b�N�X */
            dialogIcon = CreateWindow(
                "COMBOBOX",
                NULL, 
                WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST , 
                560 , 240 , 130 , 130 , 
                this->appBody , (HMENU)ID_LIST ,this->hInstance, NULL
            );
            // ���X�g�{�b�N�X�̑I����
            SendMessage(dialogIcon , CB_ADDSTRING , 0 , (LPARAM)TEXT("�A�C�R���Ȃ�"));
            SendMessage(dialogIcon , CB_ADDSTRING , 0 , (LPARAM)TEXT("���ӃA�C�R��"));
            SendMessage(dialogIcon , CB_ADDSTRING , 0 , (LPARAM)TEXT("���A�C�R��"));
            SendMessage(dialogIcon , CB_ADDSTRING , 0 , (LPARAM)TEXT("����A�C�R��"));
            SendMessage(dialogIcon , CB_ADDSTRING , 0 , (LPARAM)TEXT("�x���A�C�R��"));


            /* ���W�I�{�^������ */
            radio0 = CreateWindow(
                "BUTTON",
                TEXT("[OK]") ,
                WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP,
                560 , 265 , 160 , 30 ,
                this->appBody , NULL ,this->hInstance , NULL
            );

            /* ���W�I�{�^������ */
            radio1 = CreateWindow(
                "BUTTON",
                TEXT("[OK][�L�����Z��]") ,
                WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON ,
                560 , 295 , 160 , 30 ,
                this->appBody , NULL ,this->hInstance , NULL
            );

            /* ���W�I�{�^������ */
            radio2 = CreateWindow(
                "BUTTON",
                TEXT("[�͂�][������]") ,
                WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON ,
                560 , 325 , 160 , 30 ,
                this->appBody , NULL ,this->hInstance , NULL
            );


            /* ���W�I�{�^������ */
            radio3 = CreateWindow(
                "BUTTON",
                TEXT("[�͂�][������][�L�����Z��]") ,
                WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON ,
                720 , 235 , 210 , 30 ,
                this->appBody , NULL ,this->hInstance , NULL
            );

            /* ���W�I�{�^������ */
            radio4 = CreateWindow(
                "BUTTON",
                TEXT("[���~][�Ď��s][����]") ,
                WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON ,
                720 , 265 , 200 , 30 ,
                this->appBody , NULL ,this->hInstance , NULL
            );

            /* ���W�I�{�^������ */
            radio5 = CreateWindow(
                "BUTTON",
                TEXT("[�Ď��s][�L�����Z��]") ,
                WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON ,
                720 , 295 , 200 , 30 ,
                this->appBody , NULL ,this->hInstance , NULL
            );

            /* �`�F�b�N�{�b�N�X���� */
            check = CreateWindow(
                "BUTTON" ,
                TEXT("[�w���v]") ,
                WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX ,
                720 , 325 , 200 , 30 ,
                this->appBody , NULL , this->hInstance , NULL
            );

            /* �e�L�X�g�G���A���� */
            msgTit = CreateWindow(
                "EDIT",
                NULL,
                WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_LEFT | ES_AUTOHSCROLL | WS_HSCROLL,
                550,365,350,40,
                this->appBody,(HMENU)1,this->hInstance,NULL
            );

            /* �e�L�X�g�G���A�ɕ����\�� */
            SetWindowText(msgTit, TEXT("�^�C�g������"));

            /* �e�L�X�g�G���A���� */
            msgTex = CreateWindow(
                "EDIT",
                NULL,
                WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_LEFT | ES_AUTOVSCROLL | WS_VSCROLL,
                550,415,270,140,
                this->appBody,(HMENU)1,this->hInstance,NULL
            );
            /* �e�L�X�g�G���A�ɕ����\�� */
            SetWindowText(msgTex, TEXT("�e�L�X�g����"));

            /* �{�^������ */
            CreateWindow(
                "BUTTON",
                TEXT("�쐬"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
                830 , 525 , 100 , 30 ,
                this->appBody , (HMENU)ID_MSGCREATE , this->hInstance , NULL
            );

        }
        
        void __init__(std::string appTitle){ /* �����ݒ� */
            io.title(appTitle);
            io.cmd("color 0A");
        }

    public:
        void loop(){
            while(GetMessage(&this->msg , NULL , 0 , 0)){
                TranslateMessage(&this->msg);
                DispatchMessage(&this->msg);
            }
        }
        int end(){ // ������̏���
            io.cmd("cls");
            io.print("�I�����Ă��܂�");
            io.cmd("color 07");
            UnregisterClass("WINDOW",this->hInstance);
            Sleep(10);
            return this->msg.wParam;
        }
        APP(std::string appTitle = "�A�v��"){
            this->winc.style                = CS_HREDRAW | CS_VREDRAW;
            this->winc.lpfnWndProc          = this->WndProc;
            this->winc.cbClsExtra           = 0;
            this->winc.cbWndExtra           = 0;
            this->winc.hInstance            = this->hInstance;
            this->winc.hIcon                = LoadIcon(NULL , IDI_APPLICATION);
            this->winc.hCursor              = LoadCursor(NULL , IDC_ARROW);
            this->winc.hbrBackground        = (HBRUSH)CreateSolidBrush( RGB( 234, 234, 234 ) );
            this->winc.lpszMenuName         = NULL;
            this->winc.lpszClassName        = "WINDOW";

            if (!RegisterClass(&this->winc)){
                return;
            }
            this->__init__(appTitle);
            this->ui_setting();
            
        }
};

int main(int argc, char *argv[]){
    APP app("PEN");
    app.loop();
    app.end();
    return 0;
}