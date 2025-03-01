#include "ioset.hpp"

struct WindowParts{ // ���O
    HWND browserQuery, searchEngine;
    HWND fileContent, filePath; 
    HWND powerOption, powerOptionList, second;
    HWND cmdPrompt;
    HWND addHelp;
    HWND radio0, radio1, radio2, radio3, radio4, radio5;
    HWND dialogIcon, msgTit, msgTex;
};

struct WindowParts windowParts;
IOSet io("");
class APP{
    protected:
        MSG msg;
        WNDCLASS winc;
        HINSTANCE hInstance;
        HWND appBody; //HINSTANCE�̉��Ɋm��
        
    private:
        // ���i��
        static constexpr char 
            windowClassName[256]        = "WINDOW", 
            comboClassName[256]         = "COMBOBOX", 
            buttonClassName[256]        = "BUTTON", 
            textareaClassName[256]      = "EDIT";

        // �{�^��ID
        static constexpr int 
            ID_LIST                     = 2000,
            ID_STORE                    = 2001,
            ID_SHUTOP                   = 2002,
            ID_SANSHO                   = 2003,
            ID_CMDEXE                   = 2004,
            ID_TIME                     = 2005,
            ID_MSGCREATE                = 2006,
            ID_SEARCH                   = 2007,
            ID_SEARCHBOX                = 2008,
            ID_CMDBOX                   = 2009,
            ID_PATH                     = 2010;
        
        static LRESULT CALLBACK WndProc(HWND window , UINT requestMsg , WPARAM wp , LPARAM lp) {
            int msgVal, mbButton;
            int shutsec;
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
                    if(windowParts.powerOption ==(HWND)lp){
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
                    TextOut(
                        hdc, 
                        280 , 75 , 
                        std::string(io.pwd() + " >   ").c_str() ,
                        std::string(io.pwd() + " >   ").size()
                    );
                    //�e�L�X�g�ǉ�


                    EndPaint(window , &ps);/* �e�L�X�g�����I�� */
                    break;

                case WM_COMMAND:{ /*�{�^���������Ƃ��̑���*/
                    // �I�����ꂽ���𒲂ׂ�
                    switch (LOWORD(wp)){
                        case 1:
                            SetWindowText(window,"�e�L�X�g�����");
                            break;

                        case APP::ID_PATH:
                            SetWindowText(window,"�Q�Ƃ������t�@�C���̃p�X�̓���");
                            break;

                        case APP::ID_SEARCHBOX:
                            SetWindowText(window,"�����{�b�N�X");
                            break;

                        case APP::ID_LIST:
                            SetWindowText(window,"���X�g����I��");
                            break;

                        case APP::ID_CMDBOX:
                            SetWindowText(window,"�R�}���h����");
                            break;

                        case APP::ID_TIME:
                            SetWindowText(window, "���s����܂ł̎���(�b��)�����");
                            break;

                        case APP::ID_SHUTOP:
                            SetWindowText(window,"���s");
                            GetWindowText(windowParts.second , secondVal , 32768);
                            shutsec = atoi(secondVal);
                            switch(SendMessage(windowParts.powerOptionList, CB_GETCURSEL, 0, 0)){
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

                        case APP::ID_STORE:
                            SetWindowText(window,"�ύX�̕ۑ�");
                            msgVal = io.MsgBox("�m�F","���e��ۑ����܂����H",4,3,0);
                            switch (msgVal){
                                case IDYES:
                                    GetWindowText(windowParts.fileContent , textVal , sizeof(textVal));
                                    GetWindowText(windowParts.filePath , fileVal , sizeof(fileVal));
                                    io.write_file(std::string(fileVal), std::string(textVal), "w");
                            }
                            break;

                        case APP::ID_SANSHO:
                            SetWindowText(window,"�Q��");
                            GetWindowText(windowParts.filePath , fileVal , sizeof(fileVal));
                            fileData = io.read_file(std::string(fileVal));
                            SetWindowText(windowParts.fileContent, fileData.c_str());
                            break;

                        case APP::ID_CMDEXE:
                            SetWindowText(window,"�R�}���h�v�����v�g���s");
                            GetWindowText(windowParts.cmdPrompt, cmdVal, sizeof(cmdVal));
                            io.cmd(std::string(cmdVal));/* ���s */
                            break;

                        case APP::ID_MSGCREATE:
                            SetWindowText(window,"���b�Z�[�W�{�b�N�X����");

                            GetWindowText(windowParts.msgTex , tex , sizeof(tex));
                            GetWindowText(windowParts.msgTit , tit , sizeof(tit));

                            if      (SendMessage(windowParts.radio0, BM_GETCHECK, 0, 0))    mbButton = MB_OK;
                            else if (SendMessage(windowParts.radio1, BM_GETCHECK, 0, 0))    mbButton = MB_OKCANCEL;
                            else if (SendMessage(windowParts.radio2, BM_GETCHECK, 0, 0))    mbButton = MB_YESNO;
                            else if (SendMessage(windowParts.radio3, BM_GETCHECK, 0, 0))    mbButton = MB_YESNOCANCEL;
                            else if (SendMessage(windowParts.radio4, BM_GETCHECK, 0, 0))    mbButton = MB_ABORTRETRYIGNORE;
                            else if (SendMessage(windowParts.radio5, BM_GETCHECK, 0, 0))    mbButton = MB_RETRYCANCEL;

                            mbTitle     = std::string(tit);
                            mbText      = std::string(tex);

                            io.MsgBox(
                                mbTitle, 
                                mbText, 
                                mbButton, 
                                SendMessage(windowParts.dialogIcon, CB_GETCURSEL, 0, 0), 
                                SendMessage(windowParts.addHelp, BM_GETCHECK, 0, 0)
                            ); 
                            break;

                        case APP::ID_SEARCH:
                            SetWindowText(window,"����");
                            switch(SendMessage(windowParts.searchEngine, CB_GETCURSEL, 0, 0)){
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
                            GetWindowText(windowParts.browserQuery , searchVal , sizeof(searchVal));
                            url = "start www."+ url +"search?q=" + std::string(searchVal);
                            io.cmd(url);
                            break;

                        default:
                            SetWindowText(window, io.title("").c_str());
                    }

                }
            }
            return DefWindowProc(window, requestMsg, wp, lp);
        }

        void ui_setting(){
            HWND textEdit, cmdGroup, dialogGroup, dialogSettings;

            this->appBody = CreateWindow( /* Window���� */
                this->winc.lpszClassName,
                TEXT(io.title("").c_str()),
                (WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX) | WS_VISIBLE ,
                100 , 10 ,970 , 610 ,
                NULL , NULL , this->hInstance , NULL
            );
            
            /* ���X�g�{�b�N�X */
            windowParts.searchEngine = CreateWindow(
                this->comboClassName,
                NULL, 
                WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST , 
                20 , 10 , 150 , 150 , 
                this->appBody , (HMENU)APP::ID_LIST ,this->hInstance, NULL
            );
            // ���X�g�{�b�N�X�̑I����
            SendMessage(windowParts.searchEngine , CB_ADDSTRING , 0 , (LPARAM)TEXT("Google"));
            SendMessage(windowParts.searchEngine , CB_ADDSTRING , 0 , (LPARAM)TEXT("YouTube"));
            SendMessage(windowParts.searchEngine , CB_ADDSTRING , 0 , (LPARAM)TEXT("Bing"));
            SendMessage(windowParts.searchEngine , CB_ADDSTRING , 0 , (LPARAM)TEXT("Twitter"));


            /* �e�L�X�g�G���A���� */
            windowParts.browserQuery = CreateWindow(
                this->textareaClassName,
                NULL,
                WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_LEFT | ES_AUTOHSCROLL | WS_HSCROLL,
                180,10,620,40,
                this->appBody,(HMENU)APP::ID_SEARCHBOX,this->hInstance,NULL
            );

            SetWindowText(windowParts.browserQuery, TEXT("����")); /* �e�L�X�g�G���A�ɕ����\�� */

            /* �{�^������ */
            CreateWindow(
                this->buttonClassName,
                TEXT("search"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
                820 , 10 , 100 , 30 ,
                this->appBody , (HMENU)APP::ID_SEARCH , this->hInstance , NULL
            );    

            /* �O���[�v�{�b�N�X���� */
            windowParts.powerOption = CreateWindow(
                this->buttonClassName, 
                "�d���I�v�V����",
                WS_CHILD | WS_VISIBLE | BS_GROUPBOX ,
                20 , 50 , 240 , 120 ,
                this->appBody , NULL , this->hInstance , NULL
            );


            /* ���X�g�{�b�N�X */
            windowParts.powerOptionList = CreateWindow(
                this->comboClassName,
                NULL, 
                WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST , 
                40 , 75 , 200 , 200 , 
                this->appBody , (HMENU)APP::ID_LIST ,this->hInstance, NULL
            );
            // ���X�g�{�b�N�X�̑I����
            SendMessage(windowParts.powerOptionList , CB_ADDSTRING , 0 , (LPARAM)TEXT("���O�I�t"));
            SendMessage(windowParts.powerOptionList , CB_ADDSTRING , 0 , (LPARAM)TEXT("�V���b�g�_�E��"));
            SendMessage(windowParts.powerOptionList , CB_ADDSTRING , 0 , (LPARAM)TEXT("�ċN��"));

            /* �e�L�X�g�G���A���� */
            windowParts.second = CreateWindow(
                this->textareaClassName,
                NULL,
                WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_RIGHT | ES_NUMBER,
                40,110,30,20,
                this->appBody,(HMENU)APP::ID_TIME,this->hInstance,NULL
            );

            /* �e�L�X�g�G���A�ɕ����\�� */
            SetWindowText(windowParts.second, TEXT("30"));

            /* �{�^������ */
            CreateWindow(
                this->buttonClassName,
                TEXT("����"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
                140 , 130 , 100 , 30 ,
                this->appBody , (HMENU)APP::ID_SHUTOP , this->hInstance , NULL
            );

            /* �O���[�v�{�b�N�X���� */
            textEdit = CreateWindow(
                this->buttonClassName , 
                "�e�L�X�g�ҏW",
                WS_CHILD | WS_VISIBLE | BS_GROUPBOX ,
                20 , 190 , 500 , 380 ,
                this->appBody , NULL , this->hInstance , NULL
            );

            /* �e�L�X�g�G���A���� */
            windowParts.filePath = CreateWindow(
                this->textareaClassName,
                NULL,
                WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_LEFT | ES_AUTOHSCROLL | WS_HSCROLL,
                40,215,460,40,
                this->appBody,(HMENU)APP::ID_PATH,this->hInstance,NULL
            );

            /* �e�L�X�g�G���A�ɕ����\�� */
            SetWindowText(windowParts.filePath, io.pwd().c_str());

            /* �{�^������ */
            CreateWindow(
                this->buttonClassName,
                TEXT("�Q��"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
                370 , 260 , 100 , 30 ,
                this->appBody , (HMENU)APP::ID_SANSHO , this->hInstance , NULL
            );


            /* �e�L�X�g�G���A���� */
            windowParts.fileContent = CreateWindow(
                this->textareaClassName,
                NULL,
                WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_LEFT | ES_AUTOVSCROLL | WS_VSCROLL,
                40,310,400,200,
                this->appBody,(HMENU)1,this->hInstance,NULL
            );

            /* �e�L�X�g�G���A�ɕ����\�� */
            SetWindowText(windowParts.fileContent, TEXT("�e�L�X�g"));

            /* �{�^������ */
            CreateWindow(
                this->buttonClassName,
                TEXT("�ۑ�"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
                370 , 520 , 100 , 30 ,
                this->appBody , (HMENU)APP::ID_STORE , this->hInstance , NULL
            );

            /* �O���[�v�{�b�N�X���� */
            cmdGroup = CreateWindow(
                this->buttonClassName, 
                "�R�}���h�v�����v�g",
                WS_CHILD | WS_VISIBLE | BS_GROUPBOX ,
                270 , 50 , 670 , 120 ,
                this->appBody , NULL , this->hInstance , NULL
            );

            /* �e�L�X�g�G���A���� */
            windowParts.cmdPrompt = CreateWindow(
                this->textareaClassName,
                NULL,
                WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_LEFT | ES_AUTOHSCROLL | WS_HSCROLL,
                280 + 8*(std::string(io.pwd() + " >   ").size()), 75, 910-(280 + 8*(std::string(io.pwd() + " >   ").size()) - 4), 40,
                this->appBody,(HMENU)APP::ID_CMDBOX,this->hInstance,NULL
            );

            /* �{�^������ */
            CreateWindow(
                this->buttonClassName,
                TEXT("���s"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
                720 , 125 , 100 , 30 ,
                this->appBody , (HMENU)APP::ID_CMDEXE , this->hInstance , NULL
            );

            /* �O���[�v�{�b�N�X���� */
            dialogGroup = CreateWindow(
                this->buttonClassName, 
                "���b�Z�[�W�{�b�N�X�쐬",
                WS_CHILD | WS_VISIBLE | BS_GROUPBOX ,
                535 , 190 , 405 , 380 ,
                this->appBody , NULL , this->hInstance , NULL
            );

            /* �O���[�v�{�b�N�X���� */
            dialogSettings = CreateWindow(
                this->buttonClassName ,
                "�ݒ�" ,
                WS_CHILD | WS_VISIBLE | BS_GROUPBOX  ,
                545 , 213 , 390 , 145 ,
                this->appBody , NULL , this->hInstance , NULL
            );

            /* ���X�g�{�b�N�X */
            windowParts.dialogIcon = CreateWindow(
                this->comboClassName,
                NULL, 
                WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST , 
                560 , 240 , 130 , 130 , 
                this->appBody , (HMENU)APP::ID_LIST ,this->hInstance, NULL
            );
            // ���X�g�{�b�N�X�̑I����
            SendMessage(windowParts.dialogIcon , CB_ADDSTRING , 0 , (LPARAM)TEXT("�A�C�R���Ȃ�"));
            SendMessage(windowParts.dialogIcon , CB_ADDSTRING , 0 , (LPARAM)TEXT("���ӃA�C�R��"));
            SendMessage(windowParts.dialogIcon , CB_ADDSTRING , 0 , (LPARAM)TEXT("���A�C�R��"));
            SendMessage(windowParts.dialogIcon , CB_ADDSTRING , 0 , (LPARAM)TEXT("����A�C�R��"));
            SendMessage(windowParts.dialogIcon , CB_ADDSTRING , 0 , (LPARAM)TEXT("�x���A�C�R��"));


            /* ���W�I�{�^������ */
            windowParts.radio0 = CreateWindow(
                this->buttonClassName,
                TEXT("[OK]") ,
                WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP,
                560 , 265 , 160 , 30 ,
                this->appBody , NULL ,this->hInstance , NULL
            );

            /* ���W�I�{�^������ */
            windowParts.radio1 = CreateWindow(
                this->buttonClassName,
                TEXT("[OK][�L�����Z��]") ,
                WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON ,
                560 , 295 , 160 , 30 ,
                this->appBody , NULL ,this->hInstance , NULL
            );

            /* ���W�I�{�^������ */
            windowParts.radio2 = CreateWindow(
                this->buttonClassName,
                TEXT("[�͂�][������]") ,
                WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON ,
                560 , 325 , 160 , 30 ,
                this->appBody , NULL ,this->hInstance , NULL
            );


            /* ���W�I�{�^������ */
            windowParts.radio3 = CreateWindow(
                this->buttonClassName,
                TEXT("[�͂�][������][�L�����Z��]"),
                WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON ,
                720 , 235 , 210 , 30 ,
                this->appBody , NULL ,this->hInstance , NULL
            );

            /* ���W�I�{�^������ */
            windowParts.radio4 = CreateWindow(
                this->buttonClassName,
                TEXT("[���~][�Ď��s][����]"),
                WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON ,
                720 , 265 , 200 , 30 ,
                this->appBody , NULL ,this->hInstance , NULL
            );

            /* ���W�I�{�^������ */
            windowParts.radio5 = CreateWindow(
                this->buttonClassName,
                TEXT("[�Ď��s][�L�����Z��]"),
                WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON ,
                720 , 295 , 200 , 30 ,
                this->appBody , NULL ,this->hInstance , NULL
            );

            /* �`�F�b�N�{�b�N�X���� */
            windowParts.addHelp = CreateWindow(
                this->buttonClassName ,
                TEXT("[�w���v]") ,
                WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX ,
                720 , 325 , 200 , 30 ,
                this->appBody , NULL , this->hInstance , NULL
            );

            /* �e�L�X�g�G���A���� */
            windowParts.msgTit = CreateWindow(
                this->textareaClassName,
                NULL,
                WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_LEFT | ES_AUTOHSCROLL | WS_HSCROLL,
                550,365,350,40,
                this->appBody,(HMENU)1,this->hInstance,NULL
            );

            /* �e�L�X�g�G���A�ɕ����\�� */
            SetWindowText(windowParts.msgTit, TEXT("�^�C�g������"));

            /* �e�L�X�g�G���A���� */
            windowParts.msgTex = CreateWindow(
                this->textareaClassName,
                NULL,
                WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_LEFT | ES_AUTOVSCROLL | WS_VSCROLL,
                550,415,270,140,
                this->appBody,(HMENU)1,this->hInstance,NULL
            );
            /* �e�L�X�g�G���A�ɕ����\�� */
            SetWindowText(windowParts.msgTex, TEXT("�e�L�X�g����"));

            /* �{�^������ */
            CreateWindow(
                this->buttonClassName,
                TEXT("�쐬"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
                830 , 525 , 100 , 30 ,
                this->appBody , (HMENU)APP::ID_MSGCREATE , this->hInstance , NULL
            );

        }
        
        void __init__(std::string appTitle){ /* �����ݒ� */
            io.title(appTitle);
            io.cmd("color 0A");
        }

    public:
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
            this->winc.lpszClassName        = this->windowClassName;

            if (!RegisterClass(&this->winc)){
                return;
            }
            this->__init__(appTitle);
            this->ui_setting();
            
        }

        ~APP() noexcept{ // �f�X�g���N�^�� public
            io.cmd("cls");
            UnregisterClass(
                this->winc.lpszClassName,
                this->hInstance
            );
            io.cmd("color 07");
        }

        void loop(){
            while(GetMessage(&this->msg , NULL , 0 , 0)){
                TranslateMessage(&this->msg);
                DispatchMessage(&this->msg);
            }
        }
};


int main(int argc, char *argv[]){
    APP app("PEN");
    app.loop();
    return 0;
}