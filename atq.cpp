#include <bits/stdc++.h>
using namespace std;

struct problem {
    string date;
    string name;
    string url;

    problem(string date, string name, string url) : date(date), name(name), url(url) {
    }
};

bool comp_problem(problem a, problem b) {
    return a.date < b.date;
}

// 22/04/27 のようなフォーマットで実行時の日付を取得
string get_today_date() {
    time_t t = time(nullptr);
    const tm *localTime = localtime(&t);

    std::stringstream s;
    s << setw(2) << setfill('0') << localTime->tm_year - 100;
    s << '/';
    s << setw(2) << setfill('0') << localTime->tm_mon + 1;
    s << '/';
    s << setw(2) << setfill('0') << localTime->tm_mday;

    return s.str();
}

// url の末尾から問題名を取得
// 例えば https://atcoder.jp/contests/abc192/tasks/abc192_e なら ABC192_E
string get_problem_name(string url) {
    string name = "";

    for (int j = url.size() - 1; j >= 0; j--) {
        char c = url[j];
        if (c == '/') {
            break;
        }
        else if ('a' <= c and c <= 'z') {
            name.push_back(toupper(c));
        }
        else {
            name.push_back(c);
        }
    }

    reverse(name.begin(), name.end());

    return name;
}

// 入力ファイル queue_1 から problem の配列を作成
void read_file(ifstream &input_file, vector<problem> &problems) {
    string index;
    string name;
    string date;
    string url;

    while (input_file >> index >> date >> name >> url) {
        problems.push_back(problem(date, name, url));
    }
}

// 配列 problems に標準入力から要素を追加
void push_problem(vector<problem> &problems) {
    string date;
    string url;

    cin >> date >> url;

    date.insert(2, "/");
    date.insert(5, "/");

    string name = get_problem_name(url);

    problems.push_back(problem(date, name, url));
}

// pop した瞬間の状態をコンソール出力，pop した行も表示
void pop_out_console(int index, vector<problem> &problems) {
    cout << endl;
    for (int i = problems.size() - 1; 0 <= i; i--) {
        if (i == index - 1) {
            cout << "\033[33m"
                 << "~~~~~~~~~~~~~~~~~~~~~~~~ erased row ~~~~~~~~~~~~~~~~~~~~~~~~" << endl
                 << endl
                 << "\033[m";
            continue;
        }
        auto [date, name, url] = problems[i];

        cout << i + 1 << ", " << date << "  " << name << ' ' << url << endl << endl;
    }
}

// 配列 problems をコンソール出力．期限超過分は赤色を着色
void out_console(vector<problem> &problems) {
    cout << endl;
    string today_date = get_today_date();

    for (int i = problems.size() - 1; 0 <= i; i--) {
        auto [date, name, url] = problems[i];

        if (date <= today_date) {
            cout << "\033[31m" << i + 1 << ", " << date << "  " << name << ' ' << url << endl << endl << "\033[m";
        }
        else {
            cout << i + 1 << ", " << date << "  " << name << ' ' << url << endl << endl;
        }
    }
}

// 出力ファイル queue_1 に配列 problems を出力，入力ファイルと出力ファイルは同じことに注意
void out_file(ofstream &output_file, vector<problem> &problems) {
    for (int i = 0; i < problems.size(); i++) {
        auto [date, name, url] = problems[i];

        output_file << i + 1 << ", " << date << "  " << name << ' ' << url << "\n\n";
    }
}

// 新しい問題を追加
void push(vector<problem> &problems) {
    push_problem(problems);

    sort(problems.begin(), problems.end(), comp_problem);

    out_console(problems);
}

// キューの中身を見る
void look(vector<problem> &problems) {
    sort(problems.begin(), problems.end(), comp_problem);

    out_console(problems);
}

// 問題番号を指定して削除
void pop(int index, vector<problem> &problems) {
    cout << "Do you erase " << index << ", " << problems[index - 1].name << "?" << endl;
    cout << "please press y or n." << endl;

    string ans;
    cin >> ans;

    if (ans == "n") {
        return;
    }
    else if (ans == "y") {
        pop_out_console(index, problems);
        cout << "\n\n\n";
        problems.erase(problems.begin() + (index - 1));
        out_console(problems);
    }
}

int main(int argc, char *argv[]) {
    vector<problem> problems;

    // ファイル入力
    ifstream input_file("/home/user01/atcoder/command/queue_1.txt");
    read_file(input_file, problems);

    // コマンドライン引数によって場合分け
    string command = (string)argv[1];
    if (command == "push") {
        push(problems);
    }
    else if (command == "look") {
        look(problems);
    }
    else if (command == "pop" and argc == 3) {
        pop(stoi((string)argv[2]), problems);
    }
    else {
        cout << "command eroor" << endl;
    }

    // ファイル出力
    ofstream output_file("/home/user01/atcoder/command/queue_1.txt");
    out_file(output_file, problems);
}
