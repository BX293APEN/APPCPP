#define _CRT_SECURE_NO_WARNINGS //SDLチェック無効

#include <cstdio> 
#include <cstdlib> 
#include <iostream> 
#include <cstring> 
#include <string>
#include <algorithm> 
#include <iterator> 
#include <fstream> 
#include <istream> 
#include <sstream> 
#include <vector> 
#include <tuple> 
#include <iomanip> 
#include <thread> 
#include <future>
#include <ctime>
#include <Windows.h>
#include <shellapi.h>
#include <tchar.h>
#include <direct.h>
#include <chrono>
#include <random>
#include <unordered_map>

#define _USE_MATH_DEFINES //数値演算定数を定義
#include <cmath>

#define OPEN_PROCESS_TOKEN (TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY) // アクセス権の定数

class str : public std::string{
    public:
        using std::string::string;
        std::string replace_all(std::string target, std::string replacement){
            std::string::size_type pos = 0;
            std::string stringText = *this;
            while ((pos = stringText.find(target, pos)) != std::string::npos) {
                stringText.replace(pos, target.length(), replacement);
                pos += replacement.length();
            }
            return stringText;
        }

        std::vector<std::string> split(std::string del) {
            int first = 0;
            int i = 0;
            std::string stringText = *this;
            std::vector<std::string> result;
            std::vector<int> subStrNum = this->find_all(del);
            while (first < stringText.size()) {
                std::string subStr(stringText, first, subStrNum[i] - first);
                result.push_back(subStr); //配列の末尾に要素を追加する
                first = subStrNum[i] + 1;
                i++;
            }
            return result;
        }

        std::vector<int> find_all(std::string subStr) {
            std::vector<int> result;
            std::string stringText = *this;
            int subStrSize = subStr.size();
            int pos = stringText.find(subStr);
            while (pos != std::string::npos) {
                result.push_back(pos);
                pos = stringText.find(subStr, pos + subStrSize);
            }
            return result;
        }
        std::string to_string(){
            return (std::string)*this;
        }

        str& operator+(const str &targetText) {
            str strText = targetText;
            for(auto& s : strText){
                this -> push_back(s);
            }
            return *this;
        }

        str& operator= (std::string target){
            std::string targetString = target;
            this -> clear();
            for(auto& s : targetString){
                this -> push_back(s);
            }
            return *this;
        }
        
};

class vec : public std::vector<std::string>{
    public:
        using std::vector<std::string>::vector;
        std::string to_str (){
            std::string ans = "";
            std::vector<std::string> sourceVec = *this;
            for(std::string& s : sourceVec){
                ans = ans + s;
            }
            return ans;
        }
        std::vector<std::string> to_vec (){
            return (std::vector<std::string>)*this;
        }

        int vec_compare(std::vector<std::string> target){
            std::vector<std::string> targetVec = target;
            std::vector<std::string> sourceVec = *this;
            std::sort(targetVec.begin(), targetVec.end());
            std::sort(sourceVec.begin(), sourceVec.end());
            return std::equal(targetVec.begin(), targetVec.end(), sourceVec.begin(), sourceVec.end());
        }

        vec& operator= (std::vector<std::string> target){
            std::vector<std::string> targetVec = target;
            this -> clear();
            for(std::string& v : targetVec){
                this -> push_back(v);
            }
            return *this;
        }

        vec& operator= (std::vector<int> targetVec){
            this -> clear();
            for(int& v : targetVec){
                this -> push_back(std::to_string(v));
            }
            return *this;
        }

        vec& operator= (std::vector<double> targetVec){
            this -> clear();
            for(double& v : targetVec){
                this -> push_back(std::to_string(v));
            }
            return *this;
        }
};

class IOSet {
    protected:
        std::string appTitle; //コマンドプロンプトのタイトル
        std::string pwdPath; //カレントディレクトリ
        std::string appPath; //ファイルパス
        std::string appPathNoEX; //ファイルパス(拡張子無し)
    
    public:
        IOSet(std::string text = "タイトル") {
            std::string cmdLn;
            char path[256];
            this->appTitle = text;
            if (this->appTitle != ""){
                this->title(this->appTitle);
            }
            GetCurrentDirectory(sizeof(path), path);
            this->pwdPath = path;
            GetModuleFileName(NULL, path, sizeof(path));
            this->appPath = path;
            str pathData(path);
            this->appPathNoEX = pathData.replace_all(".exe", "");
        }

        std::string title(std::string stringText){
            if (stringText != ""){
                this->appTitle = stringText;
                cmd("title " + this->appTitle);
            }
            return this->appTitle;
        }
        
        std::string print(std::string stringText, std::string stringEnd = "\n") {
            std::cout << stringText << stringEnd << std::flush;
            return stringText;
        }
        std::string input(std::string stringPrompt = "") {
            std::string inputText;
            if (stringPrompt != ""){
                std::cout << stringPrompt << std::endl; // 「std::endl」 \n とバッファをフラッシュ
            }
            std::getline(std::cin, inputText);
            return inputText;
        }
        std::string cmd(std::string stringCmd) {
            system(stringCmd.c_str());
            return stringCmd;
        }
        int create_process_cmd(std::string stringCmd){
            std::string cmdLog = "";
            cmdLog = stringCmd;
            STARTUPINFO si;
            PROCESS_INFORMATION pi;
            ZeroMemory( &si, sizeof(si) );
            si.cb = sizeof(si);
            ZeroMemory( &pi, sizeof(pi) );
            // Start the child process. 
            CreateProcess( 
                NULL,   // No module name (use command line)
                TEXT((char*)cmdLog.c_str()),        // Command line
                NULL,           // Process handle not inheritable
                NULL,           // Thread handle not inheritable
                FALSE,          // Set handle inheritance to FALSE
                0,              // No creation flags
                NULL,           // Use parent's environment block
                NULL,           // Use parent's starting directory 
                &si,            // Pointer to STARTUPINFO structure
                &pi            // Pointer to PROCESS_INFORMATION structure
            );

            Sleep(100); //起動出来るまで待つ
        
            // Close process and thread handles. 
            CloseHandle( pi.hProcess );
            CloseHandle( pi.hThread );

            return 0;
        }

        // プロセスハンドルから特権名を有効/無効
        BOOL ProcessPrivilegeName( HANDLE hProcess, LPCTSTR lpPrivilegeName, BOOL bEnable ){
            BOOL                bSuccess = FALSE; // 戻り値(成功/失敗)
            HANDLE              hToken;     // アクセストークンのハンドル
            LUID                Luid;       // LUID(ローカル・ユニークID)
            DWORD               dwSize;     // 特権トークン容量(変更前の特権)
            TOKEN_PRIVILEGES    OldPriv;    // 特権トークン情報(変更前の特権)
            TOKEN_PRIVILEGES    NewPriv;    // 特権トークン情報(新しい特権)

            // アクセストークンのハンドルを取得
            if ( OpenProcessToken(hProcess,OPEN_PROCESS_TOKEN,&hToken) ){
                if ( LookupPrivilegeValue(NULL,lpPrivilegeName,&Luid) ){    // 特権名のLUIDを取得
                    NewPriv.PrivilegeCount              = 1;                // 特権数
                    NewPriv.Privileges[0].Luid          = Luid;             // 識別子
                    NewPriv.Privileges[0].Attributes    = bEnable ? SE_PRIVILEGE_ENABLED : 0;

                    // 特権トークン状態の有効/無効
                    if ( AdjustTokenPrivileges(hToken,FALSE,&NewPriv,sizeof(TOKEN_PRIVILEGES),&OldPriv,&dwSize) ){
                        if ( GetLastError() == ERROR_SUCCESS ){
                            bSuccess = TRUE;
                        }
                    }
                }
                CloseHandle( hToken );
            }
            return bSuccess;
        }
        int shutdown(std::string op,int second){
            int uFlag;
            if (op == "logoff"){
                uFlag = EWX_LOGOFF;
            }
            else if(op == "shutdown"){
                uFlag = EWX_SHUTDOWN;
            }
            else if(op == "reboot"){
                uFlag = EWX_REBOOT;
            }
            Sleep(second * 1000);

            // ログオフ/シャットダウン/再起動を実行する
            this->ProcessPrivilegeName( GetCurrentProcess(), SE_SHUTDOWN_NAME, TRUE );
            ExitWindowsEx( uFlag, 0 );
            return 0;
        }

        std::string path(int mode) {
            switch(mode){
                case 1:
                    return this->appPathNoEX;
                default:
                    return this->appPath;
            }
        }
        std::string pwd() {
            return this->pwdPath;
        }
        int MsgBox(
            std::string msgBoxTitle = "タイトル",
            std::string msgBoxText = "テキスト",
            int button = 0,
            int icon = 0,
            int help = 0
        ){
            int ans;
            int val1 = 0, val2 =  0, val3 = 0, forms = 0;
            switch(button){
                case 1:
                    val1 = MB_OKCANCEL;
                    break;
                case 2:
                    val1 = MB_ABORTRETRYIGNORE;
                    break;
                case 3:
                    val1 = MB_YESNOCANCEL;
                    break;
                case 4:
                    val1 = MB_YESNO;        
                    break;
                case 5:
                    val1 = MB_RETRYCANCEL;
                    break;
                default:
                    val1 = MB_OK;
                    break;
            }
            switch(icon){
                case 1:
                case MB_ICONWARNING:
                    val2 = MB_ICONWARNING;
                    break;
                case 2:
                case MB_ICONINFORMATION:
                    val2 = MB_ICONINFORMATION;
                    break;
                case 3:
                case MB_ICONQUESTION:
                    val2 = MB_ICONQUESTION;
                    break;
                case 4:
                case MB_ICONERROR:
                    val2 = MB_ICONERROR;
                    break;
                default:
                    break;
            }
            switch(help){
                case 1:
                case MB_HELP:
                    val3 = MB_HELP;
                    break;
                default:
                    break;
            }
            forms = val1 | val2 | val3;
            ans = MessageBox(NULL , msgBoxText.c_str(), msgBoxTitle.c_str(), forms );
            return ans;
        }
        std::string read_file(std::string filePath){
            std::ifstream ifs(filePath);
            std::string fileData = "";
            std::string str;
            if (ifs.fail()) {
                return "FALSE";
            }
            while (std::getline(ifs, str)) {
                fileData = fileData + str + "\n";
            }
            return fileData;
        }
        
        int write_file(std::string filePath, std::string data, std::string mode = "a"){
            if (mode == "a"){
                if (filePath == this->appPathNoEX + ".cpp" || filePath == this->appPath){
                    int ans;
                    ans = this->MsgBox("警告", "このプログラムの重要ファイルです。\n書き換えますか？", 4, 1, 0);
                    if (ans == IDNO){
                        this->print("アクセス拒否されました。");
                        return 2;
                    }
                }
                std::string sdata = read_file(filePath); //データ読み込み
                std::ofstream ofs(filePath); //空ファイルにする
                if (ofs.fail()){
                    this->print("FALSE");
                    return -1;
                }
                ofs << sdata << data << std::endl;
                this->print(filePath + "に追加書き込み完了しました。");
                return 0;
            }
            else if (mode == "w"){
                if (filePath == this->appPathNoEX + ".cpp" || filePath == this->appPath){
                    int ans;
                    ans = this->MsgBox("警告", "このプログラムの重要ファイルです。\nエディタで書き換えてください！！", 0, 4, 0);
                    this->print("アクセス拒否されました。");
                    return -1;
                }
                std::ofstream ofs(filePath); //空ファイルにする
                if (ofs.fail()){
                    this->print("FALSE");
                    this->print("書き込み失敗");
                    return -1;
                }
                ofs << data << std::endl;
                this->print(filePath + "に書き込み完了しました。");
                return 0;
            }
            else{
                this->print("モードが違います。");
                return -1;
            }
        }
        std::vector<int> get_now_time() {
            time_t t = time(nullptr);
            struct tm* nowTime = localtime(&t);
            std::vector<int> value = {
                nowTime->tm_year + 1900,
                nowTime->tm_mon + 1,
                nowTime->tm_mday,
                nowTime->tm_hour,
                nowTime->tm_min,
                nowTime->tm_sec
            };
            return value;
        }
        int random_num() {
            std::random_device rnd;
            return rnd();
        }
};


#define TRAY_H
#define WM_TRAY_CALLBACK_MESSAGE (WM_USER + 1)
#define WC_TRAY_CLASS_NAME "TRAY"
#define ID_TRAY_FIRST 1000
HMENU   trayMenuItem;
struct tray_menu;

struct tray {
    char *icon;        
    struct tray_menu *menu;
};

struct tray_menu {
    char *text;
    int disabled;
    int checked;

    void (*cb)(struct tray_menu *);
    void *context;

    struct tray_menu *submenu;
};

class TaskTray{ //タスクトレイ
    protected:
        WNDCLASSEX      wc;
        NOTIFYICONDATA  nid;
        HWND            hwnd;
        UINT            id;
    
    public:
        HMENU _tray_menu(struct tray_menu *m) {
            HMENU hMenu = CreatePopupMenu();
            for (; m != NULL && m->text != NULL; m++, (this->id)++) {
                if (strcmp(m->text, "-") == 0) {
                    InsertMenu(hMenu, this->id, MF_SEPARATOR, TRUE, "");
                } 
                else {
                    MENUITEMINFO item;
                    memset(&item, 0, sizeof(item));
                    item.cbSize = sizeof(MENUITEMINFO);
                    item.fMask = MIIM_ID | MIIM_TYPE | MIIM_STATE | MIIM_DATA;
                    item.fType = 0;
                    item.fState = 0;
                    if (m->submenu != NULL) {
                        item.fMask = item.fMask | MIIM_SUBMENU;
                        item.hSubMenu = this->_tray_menu(m->submenu);
                    }
                    if (m->disabled) {
                        item.fState |= MFS_DISABLED;
                    }
                    if (m->checked) {
                        item.fState |= MFS_CHECKED;
                    }
                    item.wID = this->id;
                    item.dwTypeData = m->text;
                    item.dwItemData = (ULONG_PTR)m;

                    InsertMenuItem(hMenu, this->id, TRUE, &item);
                }
            }
            return hMenu;
        }

        int tray_loop(int blocking) {
            MSG msg;
            if (blocking) {
                GetMessage(&msg, NULL, 0, 0);
            } 
            else {
                PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
            }
            if (msg.message == WM_QUIT) {
                return -1;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            return 0;
        }

        void tray_update(struct tray *taskTray) {
            HMENU prevmenu = trayMenuItem;
            trayMenuItem = this->_tray_menu(taskTray->menu);
            SendMessage(this->hwnd, WM_INITMENUPOPUP, (WPARAM)trayMenuItem, 0);
            HICON icon;
            ExtractIconEx(taskTray->icon, 0, NULL, &icon, 1);
            if (this->nid.hIcon) {
                DestroyIcon(this->nid.hIcon);
            }
            this->nid.hIcon = icon;
            Shell_NotifyIcon(NIM_MODIFY, &nid);

            if (prevmenu != NULL) {
                DestroyMenu(prevmenu);
            }
        }
        
        TaskTray(std::string menuName) {
            this->id = ID_TRAY_FIRST;
            memset(&this->wc, 0, sizeof(this->wc));
            this->wc.cbSize = sizeof(WNDCLASSEX);
            this->wc.lpfnWndProc = (WNDPROC)this->_tray_wnd_proc;
            this->wc.hInstance = GetModuleHandle(NULL);
            this->wc.lpszClassName = WC_TRAY_CLASS_NAME;
            this->wc.lpszMenuName = menuName.c_str();
            if (!RegisterClassEx(&this->wc)) {
                return;
            }

            this->hwnd = CreateWindowEx(0, WC_TRAY_CLASS_NAME, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0);
            if (this->hwnd == NULL) {
                return;
            }
            UpdateWindow(this->hwnd);

            memset(&this->nid, 0, sizeof(this->nid));
            this->nid.cbSize = sizeof(NOTIFYICONDATA);
            this->nid.hWnd = this->hwnd;
            this->nid.uID = 0;
            this->nid.uFlags = NIF_ICON | NIF_MESSAGE;
            this->nid.uCallbackMessage = WM_TRAY_CALLBACK_MESSAGE;
            lstrcpy( this->nid.szTip, TEXT(menuName.c_str())); 
            Shell_NotifyIcon(NIM_ADD, &this->nid);
            return;
        }
        void tray_exit() {
            Shell_NotifyIcon(NIM_DELETE, &this->nid);
            if (this->nid.hIcon != 0) {
                DestroyIcon(this->nid.hIcon);
            }
            if (trayMenuItem != 0) {
                DestroyMenu(trayMenuItem);
            }
            PostQuitMessage(0);
            UnregisterClass(WC_TRAY_CLASS_NAME, GetModuleHandle(NULL));
        }

        static LRESULT CALLBACK _tray_wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
            switch (msg) {
                case WM_CLOSE:
                    DestroyWindow(hwnd);
                    return 0;
                case WM_DESTROY:
                    PostQuitMessage(0);
                    return 0;
                case WM_TRAY_CALLBACK_MESSAGE:
                    switch (lparam){
                        case WM_LBUTTONUP:
                        case WM_RBUTTONUP:
                            POINT p;
                            GetCursorPos(&p);
                            SetForegroundWindow(hwnd);
                            WORD cmd = TrackPopupMenu(
                                trayMenuItem, TPM_LEFTALIGN | 
                                TPM_RIGHTBUTTON | 
                                TPM_RETURNCMD | 
                                TPM_NONOTIFY,
                                p.x, 
                                p.y, 
                                0, 
                                hwnd, 
                                NULL
                            );
                            SendMessage(hwnd, WM_COMMAND, cmd, 0);
                            return 0;
                    }
                    break;
                case WM_COMMAND:
                    if (wparam >= ID_TRAY_FIRST) {
                        MENUITEMINFO item = {
                            .cbSize = sizeof(MENUITEMINFO), .fMask = MIIM_ID | MIIM_DATA,
                        };
                        if (GetMenuItemInfo(trayMenuItem, wparam, FALSE, &item)) {
                            struct tray_menu *menu = (struct tray_menu *)item.dwItemData;
                            if (menu != NULL && menu->cb != NULL) {
                                menu->cb(menu);
                            }
                        }
                        return 0;
                    }
                    break;
            }
            return DefWindowProc(hwnd, msg, wparam, lparam);
        }
};

//TaskTray taskTray("ABC");
//void finish(tray_menu *){
//    taskTray.tray_exit();
//}
//void func1(tray_menu *){
//    IOSet io("");
//    io.create_process_cmd("winver");
//}
//
//tray tray_form ={               // タスクトレイの設定
//    (char*)"icon.ico", // タスクトレイのアイコン画像。別途用意しておく。
//    (tray_menu[]){
//        {(char*)"Settings",0,0,func1},
//        {(char*)"-"},                 // セパレータ
//        {(char*)"Quit",0,0,finish},   //終了
//        {nullptr}                     //最後の要素。必須。
//    }
//};
//int main(int argc, char *argv[]) {
//    taskTray.tray_update(&tray_form);
//    while(taskTray.tray_loop(1) == 0){} //タスクトレイが表示されている間ループ
//    return 0;
//}

//int main(int argc, char *argv[]){
//    IOSet io("APP");
//    io.cmd("notepad");
//    return 0;
//}