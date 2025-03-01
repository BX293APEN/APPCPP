#include "ioset.hpp"

struct WindowParts{ // 名前
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
        HWND appBody; //HINSTANCEの下に確保
        
    private:
        // 部品名
        static constexpr char 
            windowClassName[256]        = "WINDOW", 
            comboClassName[256]         = "COMBOBOX", 
            buttonClassName[256]        = "BUTTON", 
            textareaClassName[256]      = "EDIT";

        // ボタンID
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

            switch (requestMsg) { //Windowからのメッセージを分類
                case WM_CLOSE: /* × ボタンが押された時の操作 */
                    SetWindowText(window,"終了");
                    msgVal = io.MsgBox("確認","終了しますか？",4,3,0);
                    switch (msgVal){
                        case IDYES:
                            DestroyWindow(window);
                            PostQuitMessage(0);
                            return 0;

                        default:
                            SetWindowText(window, io.title("").c_str());
                            return 0;
                    }
                    
                case WM_CTLCOLORSTATIC: /*コントロールスタティックが返ってくる場所(グループボックス)の設定*/
                    SetBkMode((HDC)wp,TRANSPARENT); //背景を透明にして綺麗にする
                    if(windowParts.powerOption ==(HWND)lp){
                        SetTextColor((HDC)wp,RGB(255,0,0));//テキスト
                    }
                    return (LRESULT)CreateSolidBrush( RGB( 234, 234, 234 ) ); //背景色

                case WM_LBUTTONDOWN: /*左クリック押したときの操作*/
                    SetWindowText(window,"左クリック");
                    break;

                /*左クリック押したときの操作*/
                case WM_RBUTTONDOWN:
                    SetWindowText(window,"右クリック");
                    break;

                /*フォーカスを外したときの操作*/
                case EN_KILLFOCUS:
                    SetWindowText(window, io.title("").c_str());
                    break;

                /*Windowに描画する場所の設定*/
                case WM_PAINT:
                    hdc = BeginPaint(window , &ps);

                    /* テキスト生成 */
                    SetBkColor(hdc , RGB(234 , 234 , 234));
                    SetTextColor(hdc , RGB(0 , 0 , 0));
                    TextOut(hdc , 75 , 110 , TEXT("秒") , lstrlen(TEXT("秒")));

                    SetBkColor(hdc , RGB(0 , 0 , 0));
                    SetTextColor(hdc , RGB(0 , 255 , 0));
                    TextOut(
                        hdc, 
                        280 , 75 , 
                        std::string(io.pwd() + " >   ").c_str() ,
                        std::string(io.pwd() + " >   ").size()
                    );
                    //テキスト追加


                    EndPaint(window , &ps);/* テキスト生成終了 */
                    break;

                case WM_COMMAND:{ /*ボタン押したときの操作*/
                    // 選択された所を調べる
                    switch (LOWORD(wp)){
                        case 1:
                            SetWindowText(window,"テキストを入力");
                            break;

                        case APP::ID_PATH:
                            SetWindowText(window,"参照したいファイルのパスの入力");
                            break;

                        case APP::ID_SEARCHBOX:
                            SetWindowText(window,"検索ボックス");
                            break;

                        case APP::ID_LIST:
                            SetWindowText(window,"リストから選択");
                            break;

                        case APP::ID_CMDBOX:
                            SetWindowText(window,"コマンド入力");
                            break;

                        case APP::ID_TIME:
                            SetWindowText(window, "実行するまでの時間(秒数)を入力");
                            break;

                        case APP::ID_SHUTOP:
                            SetWindowText(window,"実行");
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
                            SetWindowText(window,"実行出来ません");
                            break;

                        case APP::ID_STORE:
                            SetWindowText(window,"変更の保存");
                            msgVal = io.MsgBox("確認","内容を保存しますか？",4,3,0);
                            switch (msgVal){
                                case IDYES:
                                    GetWindowText(windowParts.fileContent , textVal , sizeof(textVal));
                                    GetWindowText(windowParts.filePath , fileVal , sizeof(fileVal));
                                    io.write_file(std::string(fileVal), std::string(textVal), "w");
                            }
                            break;

                        case APP::ID_SANSHO:
                            SetWindowText(window,"参照");
                            GetWindowText(windowParts.filePath , fileVal , sizeof(fileVal));
                            fileData = io.read_file(std::string(fileVal));
                            SetWindowText(windowParts.fileContent, fileData.c_str());
                            break;

                        case APP::ID_CMDEXE:
                            SetWindowText(window,"コマンドプロンプト実行");
                            GetWindowText(windowParts.cmdPrompt, cmdVal, sizeof(cmdVal));
                            io.cmd(std::string(cmdVal));/* 実行 */
                            break;

                        case APP::ID_MSGCREATE:
                            SetWindowText(window,"メッセージボックス生成");

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
                            SetWindowText(window,"検索");
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

            this->appBody = CreateWindow( /* Window生成 */
                this->winc.lpszClassName,
                TEXT(io.title("").c_str()),
                (WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX) | WS_VISIBLE ,
                100 , 10 ,970 , 610 ,
                NULL , NULL , this->hInstance , NULL
            );
            
            /* リストボックス */
            windowParts.searchEngine = CreateWindow(
                this->comboClassName,
                NULL, 
                WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST , 
                20 , 10 , 150 , 150 , 
                this->appBody , (HMENU)APP::ID_LIST ,this->hInstance, NULL
            );
            // リストボックスの選択肢
            SendMessage(windowParts.searchEngine , CB_ADDSTRING , 0 , (LPARAM)TEXT("Google"));
            SendMessage(windowParts.searchEngine , CB_ADDSTRING , 0 , (LPARAM)TEXT("YouTube"));
            SendMessage(windowParts.searchEngine , CB_ADDSTRING , 0 , (LPARAM)TEXT("Bing"));
            SendMessage(windowParts.searchEngine , CB_ADDSTRING , 0 , (LPARAM)TEXT("Twitter"));


            /* テキストエリア生成 */
            windowParts.browserQuery = CreateWindow(
                this->textareaClassName,
                NULL,
                WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_LEFT | ES_AUTOHSCROLL | WS_HSCROLL,
                180,10,620,40,
                this->appBody,(HMENU)APP::ID_SEARCHBOX,this->hInstance,NULL
            );

            SetWindowText(windowParts.browserQuery, TEXT("検索")); /* テキストエリアに文字表示 */

            /* ボタン生成 */
            CreateWindow(
                this->buttonClassName,
                TEXT("search"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
                820 , 10 , 100 , 30 ,
                this->appBody , (HMENU)APP::ID_SEARCH , this->hInstance , NULL
            );    

            /* グループボックス生成 */
            windowParts.powerOption = CreateWindow(
                this->buttonClassName, 
                "電源オプション",
                WS_CHILD | WS_VISIBLE | BS_GROUPBOX ,
                20 , 50 , 240 , 120 ,
                this->appBody , NULL , this->hInstance , NULL
            );


            /* リストボックス */
            windowParts.powerOptionList = CreateWindow(
                this->comboClassName,
                NULL, 
                WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST , 
                40 , 75 , 200 , 200 , 
                this->appBody , (HMENU)APP::ID_LIST ,this->hInstance, NULL
            );
            // リストボックスの選択肢
            SendMessage(windowParts.powerOptionList , CB_ADDSTRING , 0 , (LPARAM)TEXT("ログオフ"));
            SendMessage(windowParts.powerOptionList , CB_ADDSTRING , 0 , (LPARAM)TEXT("シャットダウン"));
            SendMessage(windowParts.powerOptionList , CB_ADDSTRING , 0 , (LPARAM)TEXT("再起動"));

            /* テキストエリア生成 */
            windowParts.second = CreateWindow(
                this->textareaClassName,
                NULL,
                WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_RIGHT | ES_NUMBER,
                40,110,30,20,
                this->appBody,(HMENU)APP::ID_TIME,this->hInstance,NULL
            );

            /* テキストエリアに文字表示 */
            SetWindowText(windowParts.second, TEXT("30"));

            /* ボタン生成 */
            CreateWindow(
                this->buttonClassName,
                TEXT("決定"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
                140 , 130 , 100 , 30 ,
                this->appBody , (HMENU)APP::ID_SHUTOP , this->hInstance , NULL
            );

            /* グループボックス生成 */
            textEdit = CreateWindow(
                this->buttonClassName , 
                "テキスト編集",
                WS_CHILD | WS_VISIBLE | BS_GROUPBOX ,
                20 , 190 , 500 , 380 ,
                this->appBody , NULL , this->hInstance , NULL
            );

            /* テキストエリア生成 */
            windowParts.filePath = CreateWindow(
                this->textareaClassName,
                NULL,
                WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_LEFT | ES_AUTOHSCROLL | WS_HSCROLL,
                40,215,460,40,
                this->appBody,(HMENU)APP::ID_PATH,this->hInstance,NULL
            );

            /* テキストエリアに文字表示 */
            SetWindowText(windowParts.filePath, io.pwd().c_str());

            /* ボタン生成 */
            CreateWindow(
                this->buttonClassName,
                TEXT("参照"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
                370 , 260 , 100 , 30 ,
                this->appBody , (HMENU)APP::ID_SANSHO , this->hInstance , NULL
            );


            /* テキストエリア生成 */
            windowParts.fileContent = CreateWindow(
                this->textareaClassName,
                NULL,
                WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_LEFT | ES_AUTOVSCROLL | WS_VSCROLL,
                40,310,400,200,
                this->appBody,(HMENU)1,this->hInstance,NULL
            );

            /* テキストエリアに文字表示 */
            SetWindowText(windowParts.fileContent, TEXT("テキスト"));

            /* ボタン生成 */
            CreateWindow(
                this->buttonClassName,
                TEXT("保存"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
                370 , 520 , 100 , 30 ,
                this->appBody , (HMENU)APP::ID_STORE , this->hInstance , NULL
            );

            /* グループボックス生成 */
            cmdGroup = CreateWindow(
                this->buttonClassName, 
                "コマンドプロンプト",
                WS_CHILD | WS_VISIBLE | BS_GROUPBOX ,
                270 , 50 , 670 , 120 ,
                this->appBody , NULL , this->hInstance , NULL
            );

            /* テキストエリア生成 */
            windowParts.cmdPrompt = CreateWindow(
                this->textareaClassName,
                NULL,
                WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_LEFT | ES_AUTOHSCROLL | WS_HSCROLL,
                280 + 8*(std::string(io.pwd() + " >   ").size()), 75, 910-(280 + 8*(std::string(io.pwd() + " >   ").size()) - 4), 40,
                this->appBody,(HMENU)APP::ID_CMDBOX,this->hInstance,NULL
            );

            /* ボタン生成 */
            CreateWindow(
                this->buttonClassName,
                TEXT("実行"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
                720 , 125 , 100 , 30 ,
                this->appBody , (HMENU)APP::ID_CMDEXE , this->hInstance , NULL
            );

            /* グループボックス生成 */
            dialogGroup = CreateWindow(
                this->buttonClassName, 
                "メッセージボックス作成",
                WS_CHILD | WS_VISIBLE | BS_GROUPBOX ,
                535 , 190 , 405 , 380 ,
                this->appBody , NULL , this->hInstance , NULL
            );

            /* グループボックス生成 */
            dialogSettings = CreateWindow(
                this->buttonClassName ,
                "設定" ,
                WS_CHILD | WS_VISIBLE | BS_GROUPBOX  ,
                545 , 213 , 390 , 145 ,
                this->appBody , NULL , this->hInstance , NULL
            );

            /* リストボックス */
            windowParts.dialogIcon = CreateWindow(
                this->comboClassName,
                NULL, 
                WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST , 
                560 , 240 , 130 , 130 , 
                this->appBody , (HMENU)APP::ID_LIST ,this->hInstance, NULL
            );
            // リストボックスの選択肢
            SendMessage(windowParts.dialogIcon , CB_ADDSTRING , 0 , (LPARAM)TEXT("アイコンなし"));
            SendMessage(windowParts.dialogIcon , CB_ADDSTRING , 0 , (LPARAM)TEXT("注意アイコン"));
            SendMessage(windowParts.dialogIcon , CB_ADDSTRING , 0 , (LPARAM)TEXT("情報アイコン"));
            SendMessage(windowParts.dialogIcon , CB_ADDSTRING , 0 , (LPARAM)TEXT("質問アイコン"));
            SendMessage(windowParts.dialogIcon , CB_ADDSTRING , 0 , (LPARAM)TEXT("警告アイコン"));


            /* ラジオボタン生成 */
            windowParts.radio0 = CreateWindow(
                this->buttonClassName,
                TEXT("[OK]") ,
                WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP,
                560 , 265 , 160 , 30 ,
                this->appBody , NULL ,this->hInstance , NULL
            );

            /* ラジオボタン生成 */
            windowParts.radio1 = CreateWindow(
                this->buttonClassName,
                TEXT("[OK][キャンセル]") ,
                WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON ,
                560 , 295 , 160 , 30 ,
                this->appBody , NULL ,this->hInstance , NULL
            );

            /* ラジオボタン生成 */
            windowParts.radio2 = CreateWindow(
                this->buttonClassName,
                TEXT("[はい][いいえ]") ,
                WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON ,
                560 , 325 , 160 , 30 ,
                this->appBody , NULL ,this->hInstance , NULL
            );


            /* ラジオボタン生成 */
            windowParts.radio3 = CreateWindow(
                this->buttonClassName,
                TEXT("[はい][いいえ][キャンセル]"),
                WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON ,
                720 , 235 , 210 , 30 ,
                this->appBody , NULL ,this->hInstance , NULL
            );

            /* ラジオボタン生成 */
            windowParts.radio4 = CreateWindow(
                this->buttonClassName,
                TEXT("[中止][再試行][無視]"),
                WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON ,
                720 , 265 , 200 , 30 ,
                this->appBody , NULL ,this->hInstance , NULL
            );

            /* ラジオボタン生成 */
            windowParts.radio5 = CreateWindow(
                this->buttonClassName,
                TEXT("[再試行][キャンセル]"),
                WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON ,
                720 , 295 , 200 , 30 ,
                this->appBody , NULL ,this->hInstance , NULL
            );

            /* チェックボックス生成 */
            windowParts.addHelp = CreateWindow(
                this->buttonClassName ,
                TEXT("[ヘルプ]") ,
                WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX ,
                720 , 325 , 200 , 30 ,
                this->appBody , NULL , this->hInstance , NULL
            );

            /* テキストエリア生成 */
            windowParts.msgTit = CreateWindow(
                this->textareaClassName,
                NULL,
                WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_LEFT | ES_AUTOHSCROLL | WS_HSCROLL,
                550,365,350,40,
                this->appBody,(HMENU)1,this->hInstance,NULL
            );

            /* テキストエリアに文字表示 */
            SetWindowText(windowParts.msgTit, TEXT("タイトル入力"));

            /* テキストエリア生成 */
            windowParts.msgTex = CreateWindow(
                this->textareaClassName,
                NULL,
                WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_LEFT | ES_AUTOVSCROLL | WS_VSCROLL,
                550,415,270,140,
                this->appBody,(HMENU)1,this->hInstance,NULL
            );
            /* テキストエリアに文字表示 */
            SetWindowText(windowParts.msgTex, TEXT("テキスト入力"));

            /* ボタン生成 */
            CreateWindow(
                this->buttonClassName,
                TEXT("作成"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
                830 , 525 , 100 , 30 ,
                this->appBody , (HMENU)APP::ID_MSGCREATE , this->hInstance , NULL
            );

        }
        
        void __init__(std::string appTitle){ /* 初期設定 */
            io.title(appTitle);
            io.cmd("color 0A");
        }

    public:
        APP(std::string appTitle = "アプリ"){
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

        ~APP() noexcept{ // デストラクタも public
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