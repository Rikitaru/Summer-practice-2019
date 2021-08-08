#include <iostream>
//new
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <clocale>
#define TG "Время генерации "
#define TS "Время отправки "
#define TE "Время доставки "
using namespace std;
int kof = 7;

int Number_of_divisions_in_time = 5; //Количество Ячеек с выводом времени в строке Время
int Number_of_divisions_in_time_for_one = 3*kof; //Количество ячеек на одно деление по времени внутри тайм-слота 
int number_of_cells = Number_of_divisions_in_time*Number_of_divisions_in_time_for_one; //Количество ячеек на один тайм-слот

float singledeltaTime=0; //Сколько приходится времени на одну ячейку в каждом тайм-слоте
int	size_of_cell_time=0; //Суммарное количество ячеек для Времени
int mmmmm = -1;
int kkk = -1;
float timetime = 0.0;
int rr = 0;

string Main_time;//единица измерения тайм-слота
int for_timeslots_new = 0; //количество нулей после запятой
int length_for_time = 0; //длина значения тайм-слота, для красивой таблицы

float deltaTime=0;
string style1 = "Gen";
string style2 = "Busy";
string style3 = " False";

int employed=0;
vector <vector<string>>False_traffic;
vector<string> False_Node;// Названия узлов   ПЕРЕДЕЛАТЬ В STRING!!!
int nn = 0;
int index=0;
int flag0 = 0;
vector <string> False_Traffics;
vector <string> False_Traffics2;
class errors
{
public:
	vector <vector <string>> _traffic;
	vector <vector <string>> _node;
	vector <vector <string>> _switch;
};

class table
{
public:
	errors error;
	int timeSlots; // Количество Тайм-слотов
	vector <float> startTime; // Начальные времена Тайм-слотов
	vector <string> node; // Названия узлов   ПЕРЕДЕЛАТЬ В STRING!!!
	vector <vector <string>> scheduling_table; // Номера тайм-слотов, где используется узел
	vector <vector <vector <string>>> traffic_position; // Данные traffic_position
	table();
};

table::table()
{
	timeSlots = 0;
}

class tags
{
public:
	vector <string> data;
	bool allow;
	string tag;
	string openTag;
	string closeTag;
	tags(string tag, vector <string> *dataVector, table *tableData);
};

tags::tags(string tag, vector <string> *dataVector = nullptr, table *tableData = nullptr)
{
	int pos = 0;
	if(dataVector)
		data = *dataVector;
	allow = false;
	int flag = tag.find('/');
	if(flag != -1)
		tag.erase(tag.find("/"),1);
	this->tag = tag;
	if (tag == "<message>" && flag == -1)
	{
		allow = true;
		openTag = "\t<p> Сообщение: " + data[0];
		closeTag = "</p>\n";
	}
	if (tag == "<epoch>" && flag == -1)
	{
		allow = true;
		openTag = "<p>Заданные пользователем параметры:</p>\n";
		closeTag = "";
	}

	if ((tag == "<traffic>") && (flag0 == 0))
	{
		tableData->error._traffic.push_back(*dataVector);
	}

	if ((tag == "<node>") && (flag0 == 0 ))
	{
		tableData->error._node.push_back(*dataVector);
	}
	if ((tag == "<switch>") && (flag0 == 0))
	{
		tableData->error._switch.push_back(*dataVector);
	}

	if (tag == "<traffic_list>")
	{
		allow = true;
		if (flag == -1) 
		{
			openTag = "\t<ul>\n\t\tТрафики для корректировки:\n\t\t<content>\n";
			closeTag = "";
		}
		else
		{
			openTag = "";
			for (int i = 0; i < tableData->error._traffic.size(); i++) {
				closeTag += "\t\t\t<ul>ID - " + tableData->error._traffic[i][0] + ", Узел-отправитель - " + tableData->error._traffic[i][1]+ " " + tableData->error._traffic[i][2] + "<br></ul>\n";
			}
			closeTag += "\t\t</content>\n\t</ul>\n ";
		}
	}
	
	if (tag == "<node_list>")
	{
		allow = true;
		if (flag == -1)
		{
			openTag = "\t<ul>\n\t\tУзлы для корректировки:\n\t\t<content>\n";
			closeTag = "";
		}
		else
		{
			openTag = "";
			for (int i = 0; i < tableData->error._node.size(); i++) {
				closeTag += "\t\t\t<ul>ID - " + tableData->error._node[i][0] +"<br></ul>\n";
			}
			closeTag += "\t\t</content>\n\t</ul>\n";
		}
	}
	if (tag == "<switch_list>")
	{
		allow = true;
		if (flag == -1)
		{
			openTag = "\t<ul>\n\t\tКоммутаторы для корректировки:\n\t\t<content>\n";
			closeTag = "";
		}
		else
		{
			openTag = "";
			for (int i = 0; i < tableData->error._switch.size(); i++) {
				closeTag += "\t\t\t<ul>ID - " + tableData->error._switch[i][0] + "<br></ul>\n";
			}
			closeTag += "\t\t</content>\n\t</ul>\n";
			flag0 = 1;
		}
	}
	if (tag == "<user_settings>") 
	{

		allow = true;
		string s = "";
		if (data[0].find("ms", 0)<100) {
			s = " мс";
		}
		if (data[0].find("us", 0)<100) {
			s = " мкс";
		}
		if (data[0].find("ns", 0)<100) {
			s = " нс";
		}
		
		openTag = "<ul>Максимальная длительность эпохи – " + data[0].substr(0, data[0].find(' ', 0)) + s + " </ul>\n<ul>Количество временных интервалов – " + data[1] + "</ul>\n <ul>Шаг времени между временными интервалами  – " + data[2].substr(0, data[2].find(' ', 0)) + " " + s + " </ul>\n";
		closeTag = "";
	}
	if (tag == "<calculated_settings>")
	{
		allow = true;
		string s = "";
		if (data[0].find("ms", 0)<100) {
			s = " мс";
		}
		if (data[0].find("us", 0)<100) {
			s = " мкс";
		}
		if (data[0].find("ns", 0)<100) {
			s = " нс";
		}
		if (!data.empty()) {
			openTag = "<p>Результаты:</p>\n<ul>Длительность эпохи с учетом шага времени между временными интервалами – " + data[0].substr(0, data[0].find(' ', 0)) + /*data[0].substr(data[0].find(' ', 0))*/ + " " + s +" </ul>\n <ul>Длительность временного интервала с учетом шага времени между временными интервалами – " + data[1].substr(0, data[1].find(' ', 0)) + /*data[1].substr(data[1].find(' ', 0))*/ +" " + s + " </ul>\n <ul>Окно ожидания прихода тайм-кода – " + data[2] + "</ul>\n";
		}
		closeTag = "";
	}
	if (tag == "<table>" && flag == -1)
	{
		allow = true;
		openTag = "<p><ul>Таблица расписания:</ul></p>\n";
		closeTag = "";
	}
	if (tag == "<time_slot>" && flag == -1)
	{
		tableData->timeSlots++;
		int pos = 0;
		pos = data[1].find(' ', pos);
		if (data[1].find('.', 0)!=-1) {
			for_timeslots_new = data[1].find(' ', 0) - data[1].find('.', 0);
		}
		else {
			for_timeslots_new = 0;
		}

		string b = data[1].substr(0, data[1].find(' ', pos));
		Main_time = data[1].substr(data[1].find(' ', 0)+1, data[1].find(' ', 0)+2);
		length_for_time = b.length();
		tableData->startTime.push_back(stof(data[1].substr(0, data[1].find(' ', pos))));
	
		//	tableData->startTime.push_back(stoi(data[1]));
		openTag = "<p><ul>Таблица расписания:</ul></p>\n";
		closeTag = "";
	}
	if (tag == "<node>" && flag == -1 && flag0==1)
	{
		vector <string> tmpsData;
		tableData->node.push_back(data[0]);
		pos = 0;

		while (data[1].find(' ', pos) != -1)
		{
			int m = data[1].find(' ', pos);
			tmpsData.push_back(data[1].substr(pos, data[1].find(' ',pos)-pos));
			pos = data[1].find(' ', pos)+1;
		}
		tmpsData.push_back(data[1].substr(pos, data[1].length()));

		tableData->scheduling_table.push_back(tmpsData);
	}
	if (tag == "<traffic_position>" && flag == -1)
	{
		vector <vector <string>> tmpData;
		if (tableData->traffic_position.size() == tableData->node.size() && (tableData->node.size()))
		{
			tableData->traffic_position[tableData->node.size() - 1].push_back(*dataVector);
		}
		else
		{
			tmpData.push_back(*dataVector);
			float koef = 1;
			//ниже осуществлена попыька перевода при условии:
			//У тайм слота например единицы измерения us, а у времени трафика - ms, тогда подводим все под е.и тайм-слота
			/*if ((tmpData[0][1] == "ms")&(Main_time=="us")){
				koef = 1000;
			}
			if ((tmpData[0][1] == "ms")&(Main_time == "ns")) {
				koef = 1000000;
			}
			if ((tmpData[0][1] == "us")&(Main_time == "ms")) {
				koef = 1/1000;
			}
			if ((tmpData[0][1] == "us")&(Main_time == "ns")) {
				koef = 1000;
			}
			if ((tmpData[0][1] == "ns")&(Main_time == "ms")) {
				koef = 1/1000000;
			}
			if ((tmpData[0][1] == "ns")&(Main_time == "us")) {
				koef = 1 / 1000;
			}
			tmpData[0][2] = to_string((stof(tmpData[0][2])*koef));
			tmpData[0][3] = to_string((stof(tmpData[0][3])*koef));
			tmpData[0][4] = to_string((stof(tmpData[0][4])*koef));
			tmpData[0][6] = to_string((stof(tmpData[0][6])*koef));*/
			tableData->traffic_position.push_back(tmpData);

		}
	}
}

class HTMLfile
{
	ofstream fout;
	string pathFileHtml;
public:
	HTMLfile(string path, string jspath);
	~HTMLfile();
	HTMLfile& operator<< (string text);
};

HTMLfile::HTMLfile(string path, string jspath)
{
	fout.open(path);
	fout << "<!DOCTYPE html>" << endl
		<< "<head>" << endl
		<< "<meta http-equiv='X-UA-Compatible' content='IE=9'>" << endl
		<< "<meta http-equiv='content-type' content='text/html;charset=utf-8'>" << endl
		<< "<meta http-equiv='content-language' content='ru'>" << endl
		<< "<script src = 'jquery-3.4.1.min.js'></script>" << endl
		<< "<script src = 'opening.js'></script>" << endl
		<< "<script src = 'colorpicker.min.js'></script>" << endl
		<< "<script src = 'new.js'></script>" << endl
		//<< "<script src = 'worktocolor.js'></script>" << endl
		<< "<link rel = 'stylesheet' type = 'text/css' href = 'style.css'>" << endl
		<< "</head>" << endl;
}

HTMLfile::~HTMLfile()
{
	if (fout)
		fout.close();
}

HTMLfile& HTMLfile::operator<<(string text)
{
	fout << text;
	return *this;
}

class XMLfile
{
	ifstream fin;
	string pathFile;
public:
	XMLfile(string path);
	~XMLfile();
	int convert(string fullway, string jspath);
	void createTable(table& tableData, HTMLfile& newFile, vector <tags> &tagsBase);
};

XMLfile::XMLfile(string path)
{
	fin.open(path);
	pathFile = path;
}

XMLfile::~XMLfile()
{
	if (fin)
		fin.close();
}

int XMLfile::convert(string fullway, string jspath)
{
	table Tab;
	string line;
	string tmpLine;
	string tag;
	int posBegin, posEnd, i = 0;
	int flag = true;
	//////////////////
	vector <tags> tagsBase;
	vector <string> tmpData;
	/////////////////
	string pathFileHtml = pathFile;
	//pathFileHtml.replace(pathFileHtml.rfind(".xml"), 4, ".html");
	pathFileHtml = fullway;
	HTMLfile newFile(pathFileHtml, jspath);
	newFile << "<html>\n"; /*<< "<body>\n"*/;
	do
	{
		flag = true;
		getline(fin, line, '\n');
        if (line.empty())
            return -1;
		tag = line.substr(line.find("<"), line.rfind(">") + 1);
		if (line.find(" ", line.find("<")) != -1) {
			tag = line.substr(line.find("<"), line.find(" ", line.find("<"))- line.find("<")) + ">";
		}
		//cout << tag << endl;
		posEnd = 0;
		posBegin = 0;
		do
		{
			posBegin = line.find("\"", posEnd);
			posEnd = line.find("\"", posBegin + 1);
			if (posEnd != -1 && flag)
			{
				tmpLine = line.substr(posBegin + 1, posEnd - posBegin - 1);
				tmpData.push_back(tmpLine);
				//newFile << data.at(i++) << "\n";
			}
			flag = !flag;
		}while (posEnd != -1);
		tagsBase.push_back(*new tags(tag, &tmpData, &Tab));
		if (tagsBase[i].allow) 
		{
			newFile << tagsBase[i].openTag;
			newFile << tagsBase[i].closeTag;
		}
		tmpData.clear();
		i++;
	} while (line.find("</scheduler>"));



	createTable(Tab, newFile, tagsBase);
	newFile << /*"\n</body>\n" << */"</html>";
    return 1;
}

void Style(string way) {
	way += "//style.css";
	ofstream stylefile(way);
	stylefile << "table{table - layout: auto; text - align:center;}" << endl;
	stylefile << "table tbody td{text - align:center;padding: 0px 0px;}" << endl;
	stylefile << "table tbody tr:hover {background: rgb(245, 142, 59);}" << endl;
	stylefile << "table tbody td:hover {background: rgb(118, 163, 149);}" << endl;
	stylefile << "table tfoot {font-size: 12px;}" << endl;

	stylefile << "th.FalseCorrection {color:rgb(133, 0, 0);}" << endl;
	stylefile << "td.False {border:2px solid rgb(133, 0, 0);}" << endl;

	stylefile << "th.False {color:rgb(255, 0, 0)}" << endl;

	stylefile << ".Empty{background:rgb(255, 255, 255);}" << endl;
	stylefile << ".Gen{background:rgb(96, 108, 104);}" << endl;

    stylefile << ".Busy0 {background:rgb(203, 211, 213);}" << endl;
	stylefile << ".Busy1 {background:rgb(203, 211, 213);}" << endl;
	stylefile << ".Busy2 {background:rgb(108, 51, 130);}" << endl;
	stylefile << ".Busy3 {background:rgb(155,88,181);}" << endl;
	stylefile << ".Busy4 {background:rgb(108,51,130);}" << endl;
	stylefile << ".Busy5 {background:rgb(54,151,220);}" << endl;
	stylefile << ".Busy6 {background:rgb(203,211,213);}" << endl;
	stylefile << ".Busy7 {background:rgb(118,215,198);}" << endl;
	stylefile << ".Busy8 {background:rgb(23,166,136);}" << endl;
	stylefile << ".Busy9 {background:rgb(24,146,109);}" << endl;
	stylefile << ".Busy10 {background:rgb(131,226,168);}" << endl;
	stylefile << ".Busy11 {background:rgb(39,149,86);}" << endl;
	stylefile << ".Busy12 {background:rgb(247,230,161);}" << endl;
	stylefile << ".Busy13 {background:rgb(244,206,63);}" << endl;
	stylefile << ".Busy14 {background:rgb(182,149,10);}" << endl;
	stylefile << ".Busy15 {background:rgb(252,215,162);}" << endl;
	stylefile << ".Busy16 {background:rgb(248,174,67);}" << endl;
	stylefile << ".Busy17 {background:rgb(186,118,11);}" << endl;
	stylefile << ".Busy18 {background:rgb(245,202,168);}" << endl;
	stylefile << ".Busy19 {background:rgb(208,83,0);}" << endl;
	stylefile << ".Busy20 {background:rgb(146,82,21);}" << endl;
	stylefile << ".Busy21 {background:rgb(113,44,3);}" << endl;

	stylefile << "p.vertical{-webkit-transform: rotate(-90deg);-moz-transform: rotate(-90deg);-ms-transform: rotate(-90deg);-o-transform: rotate(-90deg);transform: rotate(-90deg);}" << endl;
	stylefile << ".time_begin	 {-ms-transform: rotate(-90deg);- moz - transform: rotate(-90deg);-o - transform: rotate(-90deg);-webkit - transform: rotate(-90deg);transform: rotate(-90deg);text - align: left;height: auto;vertical - align: top;color:rgb(199, 123, 36);}" << endl;
	stylefile << ".time		{-ms-transform: rotate(-90deg);- moz - transform: rotate(-90deg);-o - transform: rotate(-90deg);-webkit - transform: rotate(-90deg); transform: rotate(-90deg);text - align: left;height: auto;vertical - align: top;}" << endl;
	stylefile << ".td_bottom 	{display: table- cell; border: solid 0px; height: 100 %; text-align: center; vertical-align: middle; height: 40px;}" << endl;
	stylefile << "label {display: inline - block; cursor: pointer;}" << endl;
	stylefile << "#color-picker{margin:25px auto; width:450px;} " << endl;
	stylefile << "#color-values{display:block; list-style:none; color:#222; float:left; margin:0 0 0 15px; padding: 5px; text-align:left;}" << endl;
	stylefile << "#pcr_bg{height:135px;}" << endl;
	stylefile << ".picker-wrapper, .pcr-wrapper{position: relative; float: left;}" << endl;
	stylefile << ".picker-indicator, .pcr-indicator{position: absolute; left : 0; top : 0;}" << endl;
	stylefile << ".picker, \n.pcr{cursor: crosshair; float: left;}" << endl;
	stylefile << ".cp-default .picker {width: 200px; height: 200px;}" << endl;
	stylefile << ".cp-default .pcr {width: 30px; height: 200px;}" << endl;
	stylefile << ".cp-default .pcr-wrapper {margin-left: 10px;}" << endl;
	stylefile << ".cp-default .picker-indicator {width: 5px; height: 5px; border: 2px solid darkblue; -moz-border-radius: 4px; -o-border-radius: 4px;	-webkit-border-radius: 4px;	border-radius: 4px;opacity: .5; -ms-filter: 'progid:DXImageTransform.Microsoft.Alpha(Opacity=50)'; filter: progid:DXImageTransform.Microsoft.Alpha(Opacity = 50); filter: alpha(opacity = 50); background-color: white;}" << endl;
	stylefile << ".cp-default .pcr-indicator {width: 100%; height: 10px; left: -4px; opacity: .6; -ms-filter: 'progid:DXImageTransform.Microsoft.Alpha(Opacity=60)'; filter: progid:DXImageTransform.Microsoft.Alpha(Opacity = 60); filter: alpha(opacity = 60); border: 4px solid lightblue; -moz-border-radius: 4px; -o-border-radius: 4px; -webkit-border-radius: 4px;	border-radius: 4px; background-color: white;}" << endl;
}

void Style2(string way) {
    way += "//style.css";
    ofstream stylefile(way);
    stylefile << "table{table - layout: auto; text - align:center;}" << endl;
    stylefile << "table tbody td{text - align:center;padding: 0px 0px;}" << endl;
    stylefile << "table tbody tr:hover {background: rgb(245, 142, 59);}" << endl;
    stylefile << "table tbody td:hover {background: rgb(118, 163, 149);}" << endl;
    stylefile << "table tfoot {font-size: 12px;}" << endl;

    stylefile << "th.FalseCorrection {color:rgb(133, 0, 0);}" << endl;
    stylefile << "td.False {border:2px solid rgb(133, 0, 0);}" << endl;

    stylefile << "th.False {color:rgb(255, 0, 0)}" << endl;

    stylefile << ".Empty{background:rgb(255, 255, 255);}" << endl;
    stylefile << ".Gen{background:rgb(215,224,236);}" << endl;
    /*
    stylefile << ".Busy1 {background:rgb(135,194,234);}" << endl;
    stylefile << ".Busy2 {background:rgb(94,171,223);}" << endl;
    stylefile << ".Busy3 {background:rgb(54,151,220);}" << endl;
    stylefile << ".Busy4 {background:rgb(46,135,191);}" << endl;
    stylefile << ".Busy5 {background:rgb(43,117,166);}" << endl;
    stylefile << ".Busy6 {background:rgb(33, 98, 140);}" << endl;
    stylefile << ".Busy7 {background:rgb(43,117,166);}" << endl;
    stylefile << ".Busy8 {background:rgb(46,135,191);}" << endl;
    stylefile << ".Busy9 {background:rgb(54,151,220);}" << endl;
    stylefile << ".Busy10 {background:rgb(94,171,223);}" << endl;
    stylefile << ".Busy11 {background:rgb(135,194,234);}" << endl;
    stylefile << ".Busy12 {background:rgb(94,171,223);}" << endl;
    stylefile << ".Busy13 {background:rgb(54,151,220);}" << endl;
    stylefile << ".Busy14 {background:rgb(46,135,191);}" << endl;
    stylefile << ".Busy15 {background:rgb(43,117,166);}" << endl;
    stylefile << ".Busy16 {background:rgb(33, 98, 140);}" << endl;
    stylefile << ".Busy17 {background:rgb(43,117,166);}" << endl;
    stylefile << ".Busy18 {background:rgb(46,135,191);}" << endl;
    stylefile << ".Busy19 {background:rgb(54,151,220);}" << endl;
    stylefile << ".Busy20 {background:rgb(94,171,223);}" << endl;
    stylefile << ".Busy21 {background:rgb(135,194,234);}" << endl;
    */

    stylefile << ".Busy1  {background:rgb(46,135,191);}" << endl;
    stylefile << ".Busy2  {background:rgb(46,135,191);}" << endl;
    stylefile << ".Busy3  {background:rgb(46,135,191);}" << endl;
    stylefile << ".Busy4  {background:rgb(46,135,191);}" << endl;
    stylefile << ".Busy5  {background:rgb(46,135,191);}" << endl;
    stylefile << ".Busy6  {background:rgb(46,135,191);}" << endl;
    stylefile << ".Busy7  {background:rgb(46,135,191);}" << endl;
    stylefile << ".Busy8  {background:rgb(46,135,191);}" << endl;
    stylefile << ".Busy9  {background:rgb(46,135,191);}" << endl;
    stylefile << ".Busy10 {background:rgb(46,135,191);}" << endl;
    stylefile << ".Busy11 {background:rgb(46,135,191);}" << endl;
    stylefile << ".Busy12 {background:rgb(46,135,191);}" << endl;
    stylefile << ".Busy13 {background:rgb(46,135,191);}" << endl;
    stylefile << ".Busy14 {background:rgb(46,135,191);}" << endl;
    stylefile << ".Busy15 {background:rgb(46,135,191);}" << endl;
    stylefile << ".Busy16 {background:rgb(46,135,191);}" << endl;
    stylefile << ".Busy17 {background:rgb(46,135,191);}" << endl;
    stylefile << ".Busy18 {background:rgb(46,135,191);}" << endl;

    stylefile << "p.vertical{-webkit-transform: rotate(-90deg);-moz-transform: rotate(-90deg);-ms-transform: rotate(-90deg);-o-transform: rotate(-90deg);transform: rotate(-90deg);}" << endl;
    stylefile << ".time_begin	 {-ms-transform: rotate(-90deg);- moz - transform: rotate(-90deg);-o - transform: rotate(-90deg);-webkit - transform: rotate(-90deg);transform: rotate(-90deg);text - align: left;height: auto;vertical - align: top;color:rgb(199, 123, 36);}" << endl;
    stylefile << ".time		{-ms-transform: rotate(-90deg);- moz - transform: rotate(-90deg);-o - transform: rotate(-90deg);-webkit - transform: rotate(-90deg); transform: rotate(-90deg);text - align: left;height: auto;vertical - align: top;}" << endl;
    stylefile << ".td_bottom 	{display: table- cell; border: solid 0px; height: 100 %; text-align: center; vertical-align: middle; height: 40px;}" << endl;
    stylefile << "label {display: inline - block; cursor: pointer;}" << endl;
    stylefile << "#color-picker{margin:25px auto; width:450px;} " << endl;
    stylefile << "#color-values{display:block; list-style:none; color:#222; float:left; margin:0 0 0 15px; padding: 5px; text-align:left;}" << endl;
    stylefile << "#pcr_bg{height:135px;}" << endl;
    stylefile << ".picker-wrapper, .pcr-wrapper{position: relative; float: left;}" << endl;
    stylefile << ".picker-indicator, .pcr-indicator{position: absolute; left : 0; top : 0;}" << endl;
    stylefile << ".picker, \n.pcr{cursor: crosshair; float: left;}" << endl;
    stylefile << ".cp-default .picker {width: 200px; height: 200px;}" << endl;
    stylefile << ".cp-default .pcr {width: 30px; height: 200px;}" << endl;
    stylefile << ".cp-default .pcr-wrapper {margin-left: 10px;}" << endl;
    stylefile << ".cp-default .picker-indicator {width: 5px; height: 5px; border: 2px solid darkblue; -moz-border-radius: 4px; -o-border-radius: 4px;	-webkit-border-radius: 4px;	border-radius: 4px;opacity: .5; -ms-filter: 'progid:DXImageTransform.Microsoft.Alpha(Opacity=50)'; filter: progid:DXImageTransform.Microsoft.Alpha(Opacity = 50); filter: alpha(opacity = 50); background-color: white;}" << endl;
    stylefile << ".cp-default .pcr-indicator {width: 100%; height: 10px; left: -4px; opacity: .6; -ms-filter: 'progid:DXImageTransform.Microsoft.Alpha(Opacity=60)'; filter: progid:DXImageTransform.Microsoft.Alpha(Opacity = 60); filter: alpha(opacity = 60); border: 4px solid lightblue; -moz-border-radius: 4px; -o-border-radius: 4px; -webkit-border-radius: 4px;	border-radius: 4px; background-color: white;}" << endl;
}

void OpeningJS(string way) {
	way += "//opening.js";
	ofstream openingJS(way);
	openingJS << "$(document).ready(function() {$('[data-toggle=\"toggle\"]').change(function() {	$(this).parents().next('.hide').toggle();});});" << endl;
}

void XMLfile::createTable(table& tableData, HTMLfile& newFile, vector <tags> &tagsBase)
{
	if (tableData.timeSlots == 0) {
		newFile << "отсутсвует";
		return;
	}
	int current = 0;
	vector<vector <string>> traffic_position_temp(1); /*7 элементов всегда*/

	for (int i = 0; i < tableData.node.size(); i++) { /*Проход по узлам*/

		for (int j = 0; j < tableData.traffic_position[i].size() - 1; j++) {
			/*Проход по трафикам для каждого узла*/
			for (int k = 0; k < tableData.traffic_position[i].size() - j - 1; k++) {
				/* Ниже пузырек*/
				if (stof(tableData.traffic_position[i][k][0]) > stof(tableData.traffic_position[i][k + 1][0])) {
						traffic_position_temp[0]=tableData.traffic_position[i][k];
						tableData.traffic_position[i][k] = tableData.traffic_position[i][k + 1];
						tableData.traffic_position[i][k + 1] = traffic_position_temp[0];
				}
			}
		}
	}
	for (int i = 0; i < tableData.node.size(); i++) { /*Проход по узлам*/

		for (int j = 0; j < tableData.traffic_position[i].size() - 1; j++) {
			/*Проход по трафикам для каждого узла*/
			for (int k = 0; k < tableData.traffic_position[i].size() - j - 1; k++) {
				/* Ниже пузырек*/
				if (stof(tableData.traffic_position[i][k][0]) > stof(tableData.traffic_position[i][k + 1][0])) {
					traffic_position_temp[0] = tableData.traffic_position[i][k];
					tableData.traffic_position[i][k] = tableData.traffic_position[i][k + 1];
					tableData.traffic_position[i][k + 1] = traffic_position_temp[0];
				}
			}
		}
	}

	/*Формирование головы таблицы*/
	newFile << "<table border='1' cellspacing='0'>\n<thead>\n";
	newFile << "\t<tr>\n\t\t<th>ID узлов</th>\n";
	newFile << "\t\t<th>ID Трафиков</th>\n";
	for (int i = 0; i < tableData.timeSlots; i++)
	{
		newFile << "\t\t<th colspan='" << to_string(number_of_cells) << "'>" + to_string(i) + "</th>\n";
	}
	newFile << "\t</tr>\n";
	newFile << "</thead>\n";



	int nachalo = 0;
	/*Формирование "тел"*/
	for (int i = 0; i < tableData.node.size()/*Количество Узлов*/; i++) {

		int u = 0;
		int chet = -1;
		int ram = 0;
		for (int r = 0, e = 0; r < tagsBase.size(); r++) {

			if (tagsBase[r].tag == "<traffic_position>") {
				chet++;
				tableData.traffic_position[e][chet].push_back(to_string(r + 1));
			}
			if (tagsBase[r].tag == "<node>") {
				chet = -1;
				u++;
				if (u % 2 == 0) {
					e++;
				}
			}
			if (tagsBase[r].tag == "<table>") {
				chet = -1;
				e = 0;
				u = 0;
			}
		}

		/*Содаем массив Значений для каждой ячейки в таблице по времени*/
		deltaTime = tableData.startTime[1] - tableData.startTime[0];
		singledeltaTime = deltaTime / number_of_cells;
		size_of_cell_time = number_of_cells * tableData.timeSlots;

		int generation1 = 1;
		int generation2 = 1;
		int iii = 0;

		int beginsumm = 0;

		vector <int> sum1;
		for (int i = 0; i < tableData.node.size(); i++) {
			int sum = 0;
			for (int ii = 0; ii < tableData.traffic_position[i].size() - 1; ii++) {
				if (tableData.traffic_position[i][ii][0] != tableData.traffic_position[i][ii + 1][0]) {
					sum++;
				}
			}
			sum++;
			sum1.push_back(sum);
		}

		vector <int> summ; /*Сложная система по добыче количества различных трафиков в каждомузле и добыче количества штук каждого различного трафика*/
		for (int i = 0; i < tableData.node.size(); i++) {
			int sum = 0;
			for (int ii = 0; ii < tableData.traffic_position[i].size() - 1; ii++) {
				if (tableData.traffic_position[i][ii][0] == tableData.traffic_position[i][ii + 1][0]) {
					sum++;
				}
				else {
					sum++;
					summ.push_back(sum);
					sum = 0;
				}
			}
			sum++;
			summ.push_back(sum);
		}

		/*Определение стиля тайм-слотов, для закраски раширенной части тайм-слотов, то есть определение, правильный ли узел*/
		for (int iii = 0; iii < tableData.traffic_position[i].size(); iii++) {
			if (tableData.traffic_position[i].size() == 1) {
				if (!((stof(tableData.traffic_position[i][iii][6]) <= (tableData.startTime[tableData.timeSlots - 1] + deltaTime))
					&& (stof(tableData.traffic_position[i][iii][3]) <= (tableData.startTime[tableData.timeSlots - 1] + deltaTime))
					&& (stof(tableData.traffic_position[i][iii][2]) <= (tableData.startTime[tableData.timeSlots - 1] + deltaTime))
					/* выше - о том, что пакет не начинается запределами тамй-слотов, а также что пакет завршается до конца тайм-слотов*/
					&& (stof(tableData.traffic_position[i][iii][3]) >= stof(tableData.traffic_position[i][iii][2]))
					/* выше - о том, что Завершение пакета идет после Отправки по времени*/
					&& (stof(tableData.traffic_position[i][iii][6]) <= stof(tableData.traffic_position[i][iii][2]))
					/* выше - о том, что Генерация пакета идет до Отправки по времени*/
					&& (stof(tableData.traffic_position[i][iii][6]) <= stof(tableData.traffic_position[i][iii][3]))))
					/* выше - о том, что Генерация пакета идет до Доставки по времени*/
				{
					False_Traffics2.push_back(tableData.traffic_position[i][iii][0]);
					generation1 = 0;
					break;
				}
			}
			else {
				
				if (iii > 0) {
					if (tableData.traffic_position[i][iii][0] == tableData.traffic_position[i][iii - 1][0]) {
						if (!((stof(tableData.traffic_position[i][iii][6]) <= (tableData.startTime[tableData.timeSlots - 1] + deltaTime))
							&& (stof(tableData.traffic_position[i][iii][3]) <= (tableData.startTime[tableData.timeSlots - 1] + deltaTime))
							&& (stof(tableData.traffic_position[i][iii][2]) <= (tableData.startTime[tableData.timeSlots - 1] + deltaTime))
							/* выше - о том, что пакет не начинается запределами тамй-слотов, а также что пакет завршается до конца тайм-слотов*/
							&& (stof(tableData.traffic_position[i][iii][3]) >= stof(tableData.traffic_position[i][iii][2]))
							/* выше - о том, что Завершение пакета идет после Отправки по времени*/
							&& (stof(tableData.traffic_position[i][iii][6]) <= stof(tableData.traffic_position[i][iii][2]))
							/* выше - о том, что Генерация пакета идет до Отправки по времени*/
							&& (stof(tableData.traffic_position[i][iii][6]) <= stof(tableData.traffic_position[i][iii][3])))
							/* выше - о том, что Генерация пакета идет до Доставки по времени*/
							&& (stof(tableData.traffic_position[i][iii][2]) >= stof(tableData.traffic_position[i][iii - 1][3]))) {
							/* выше - о том, что пакет отправляется после завершения другого, то есть не происходит наложения друг на друга*/
							False_Traffics2.push_back(tableData.traffic_position[i][iii][0]);
							generation1 = 0;
							break;

						}
					}
				}
				else {
					if (!((stof(tableData.traffic_position[i][iii][6]) <= (tableData.startTime[tableData.timeSlots - 1] + deltaTime))
						&& (stof(tableData.traffic_position[i][iii][3]) <= (tableData.startTime[tableData.timeSlots - 1] + deltaTime))
						&& (stof(tableData.traffic_position[i][iii][2]) <= (tableData.startTime[tableData.timeSlots - 1] + deltaTime))
						/* выше - о том, что пакет не начинается запределами тамй-слотов, а также что пакет завршается до конца тайм-слотов*/
						&& (stof(tableData.traffic_position[i][iii][3]) >= stof(tableData.traffic_position[i][iii][2]))
						/* выше - о том, что Завершение пакета идет после Отправки по времени*/
						&& (stof(tableData.traffic_position[i][iii][6]) <= stof(tableData.traffic_position[i][iii][2]))
						/* выше - о том, что Генерация пакета идет до Отправки по времени*/
						&& (stof(tableData.traffic_position[i][iii][6]) <= stof(tableData.traffic_position[i][iii][3]))))
						/* выше - о том, что Генерация пакета идет до Доставки по времени*/
					{
						False_Traffics2.push_back(tableData.traffic_position[i][iii][0]);
						generation1 = 0;
						break;
					}
				}
			}
		}

		for (int c = 0; c < tableData.error._traffic.size(); c++) {
			if (tableData.node[i] == tableData.error._traffic[c][1]) {
				generation2 = 0;
			}
			False_Traffics.push_back(tableData.error._traffic[c][0]);
		}

		for (int c = 0; c < tableData.error._node.size(); c++) {
			if (tableData.node[i] == tableData.error._node[c][0]) {
				generation2 = 0;
			}

		}

		
		


		/*Первое Узел тело-Видимое: Начало*/
		newFile << "<tbody>\n";
		if ((generation1==1)&&(generation2==1)){//((generation)&&(generation2)) {/*Если данный узел правильный*/
			newFile << "\t<tr>\n\t\t<th>" << tableData.node[i] << "</th>\n";
			newFile << "\t\t<th align='left'> <label for='n" << to_string(i + 1) << "'> " << tableData.traffic_position[i][0][0] << " ";
		}
		else {
			if (generation2 == 0) {
				newFile << "\t<tr>\n\t\t<th class='FalseCorrection'>" << tableData.node[i] << "</th>\n";
				newFile << "\t\t<th align='left' class='FalseCorrection'> <label for='n" << to_string(i + 1) << "'> " << tableData.traffic_position[i][0][0] << " ";
			}
			else
			{
				newFile << "\t<tr>\n\t\t<th class='False'>" << tableData.node[i] << "</th>\n";
				newFile << "\t\t<th align='left' class='False'> <label for='n" << to_string(i + 1) << "'> " << tableData.traffic_position[i][0][0] << " ";

			}
		}

		for (int l1 = 0; l1 < tableData.traffic_position[i].size() - 1; l1++) {
			if (tableData.traffic_position[i][l1][0] != tableData.traffic_position[i][l1 + 1][0]) {
				newFile << tableData.traffic_position[i][l1 + 1][0] << " "; /*Вывод Айди Трафиков к нашему узлу*/
			}
		}
		newFile << "</label>\n";
		newFile << "\t\t\t<input type = 'checkbox' name = '" << to_string(i + 1) << "' id = 'n" << to_string(i + 1) << "' data-toggle = 'toggle'>\n\t\t</th>\n";

		for (int j = 0; j < tableData.timeSlots; j++) {
			int flag = 0;
			for (int k = 0; k < tableData.scheduling_table[i].size(); k++) {/*сравнение номера актуального Тайм-слота с номерами, что Заняты актуальным Узлом*/
				if (to_string(j) == tableData.scheduling_table[i][k]) {			/*Если данный узле в данном тайм-слоте используется, то рисуем*/
					flag = 1;                                        
					newFile << "\t\t<td class = 'Busy" << to_string((i % 22)+1) << "' colspan = '" << to_string(number_of_cells) << "'></td>\n";
				}
			}
			if (!flag)
				{
					newFile << "\t\t<td class = 'Empty' colspan = '" << to_string(number_of_cells) << "'></td>\n";
				}
		}
		newFile << "\t</tr>\n<body>\n";
		/*Первое тело-Видимое-Узел: Конец*/

		

		/*Начало Вывод тел циклом для Hide по ID трафикам: Начало*/
		newFile << "\t<tbody class='hide'>\n";
		for (int ii = 0, n = nn; ii < sum1[i];/*Количество трафиков в узле i */ ii++) {/*Массив размером table.id.Size, который содержит количество трафиков в каждом узле*/
			/*Сложная система по добыче нужного названия*/
			int S = -1; nn++;
			for (int p = n; p < nn; p++) {
				S += summ[p];
			}



			newFile << "\t<tr>\n\t\t<td></td>\n";

			int generationtraffics = 1;
			for (int m = 0; m < False_Traffics.size(); m++) {
				if (tableData.traffic_position[i][iii][0] == False_Traffics[m]) {
					generationtraffics = 2;
				}
			}
			for (int m = 0; m < False_Traffics2.size(); m++) {
				if (tableData.traffic_position[i][iii][0] == False_Traffics2[m]) {
					generationtraffics = 0;
				}
			}

			if (generationtraffics ==1/*Если данный трафик правильный*/) {
				newFile << "\t\t<th align='left'>" << tableData.traffic_position[i][S][0] << "</th>\n";/*Первый id трафик*/
			}
			else {
				if (generationtraffics == 2) {
					newFile << "\t\t<th align='left' class='FalseCorrection'>" << tableData.traffic_position[i][S][0] << "</th>\n";
				}
				else {
					newFile << "\t\t<th align='left' class='False'>" << tableData.traffic_position[i][S][0] << "</th>\n";

				}
			}
			
			



			float lasttime = 0;
			int BIGSUMMA = 0;
			float CELL = 0.0;
			kkk++;
				/*Рисуем новый блок так как это означает новый трафик*/
				/*Рисуем первые две ячейки(пустота и название трафика)*/
				/*Цикл ниже - рисует по трафикам блоком*/
			lasttime = tableData.startTime[0];
			BIGSUMMA = 0;
			
			for (int k = 0; k < summ[kkk]; k++, iii++) {
				
				if ((stof(tableData.traffic_position[i][iii][6]) <= (tableData.startTime[tableData.timeSlots - 1] + deltaTime))
					&& (stof(tableData.traffic_position[i][iii][3]) <= (tableData.startTime[tableData.timeSlots - 1] + deltaTime))
					&& (stof(tableData.traffic_position[i][iii][2]) <= (tableData.startTime[tableData.timeSlots - 1] + deltaTime))
					/* выше - о том, что пакет не начинается запределами тамй-слотов, а также что пакет завршается до конца тайм-слотов*/
					&& (stof(tableData.traffic_position[i][iii][2]) >= lasttime)
					&& (stof(tableData.traffic_position[i][iii][3]) >= lasttime)
					/* выше - о том, что пакет отправляется после завершения другого, то есть не происходит наложения друг на друга*/
					&& (stof(tableData.traffic_position[i][iii][3]) >= stof(tableData.traffic_position[i][iii][2]))	
					/* выше - о том, что Завершение пакета идет после Отправки по времени*/
					&& (stof(tableData.traffic_position[i][iii][6]) <= stof(tableData.traffic_position[i][iii][2]))
					/* выше - о том, что Генерация пакета идет после Отправки по времени*/
					&& (stof(tableData.traffic_position[i][iii][6]) <= stof(tableData.traffic_position[i][iii][3]))	)
					/* выше - о том, что Генерация пакета идет после Доставки по времени*/
				{
					
					//if ((stof(tableData.traffic_position[i][iii][2]) >= lasttime)&&(stof(tableData.traffic_position[i][iii][3]) >= lasttime))
					/*начало трафика*/
					/*Надо добрать пустоты до конца тайм слота и после передвинуть ласт тайм*/
					int rgen = 0;
					int rsend = 0;

					int g = BIGSUMMA / number_of_cells+1;//+1
					if (g == (tableData.timeSlots)){
						if ((BIGSUMMA % number_of_cells != 0) && (stof(tableData.traffic_position[i][iii][6]) >= tableData.startTime[g-1]+deltaTime)) {
							int rr = number_of_cells - BIGSUMMA % number_of_cells;
							newFile << "\t\t<td class = 'Empty' colspan = '" << to_string(rr) << "'></td>\n";// - оставшаяся пустота одиночек до завершения тайм-слота
							BIGSUMMA += rr;
							lasttime = tableData.startTime[g-1]+ deltaTime;
						}
					}
					else {
						if ((BIGSUMMA % number_of_cells != 0) && (stof(tableData.traffic_position[i][iii][6]) >= tableData.startTime[g])) {
							int rr = number_of_cells - BIGSUMMA % number_of_cells;
							newFile << "\t\t<td class = 'Empty' colspan = '" << to_string(rr) << "'></td>\n";// - оставшаяся пустота одиночек до завершения тайм-слота
							BIGSUMMA += rr;
							lasttime = tableData.startTime[g];
						}
					}
					if (stof(tableData.traffic_position[i][iii][6]) > lasttime) { //если до генерации существует пустота /*На первый проход tableData.startTime[0]*/
						CELL = (stof(tableData.traffic_position[i][iii][6]) - lasttime) / singledeltaTime;
						BIGSUMMA += int(floor(CELL));
						int rr = int(floor(CELL)) / number_of_cells;// - количество пачек пустоты по 15 до генерации
						for (int m = 0; m < rr; m++) {
							newFile << "\t\t<td class = 'Empty' colspan = '" << to_string(number_of_cells) << "'></td>\n";// - целая пустота ячеек пустота
						}
						lasttime = stof(tableData.traffic_position[i][iii][6]);
						rr = int(floor(CELL)) % number_of_cells;
						if (rr > 0)
						{
							newFile << "\t\t<td class = 'Empty' colspan = '" << to_string(rr) << "'></td>\n";// - оставшаяся пустота одиночек, не по 15
						/*BIGSUMMA +=rr;*/
						}
					}

					/*Генерация*/
					if (stof(tableData.traffic_position[i][iii][2]) >= lasttime) {

						float timetime = stof(tableData.traffic_position[i][iii][2]) - stof(tableData.traffic_position[i][iii][6]);
						if (lasttime > stof(tableData.traffic_position[i][iii][6])) {
							timetime = stof(tableData.traffic_position[i][iii][2]) - lasttime;
						}
						CELL = timetime / singledeltaTime;
						rgen = int(ceil(CELL));
						if (rgen == 0) {
							rgen = 1;
						}
						int truecell = round((stof(tableData.traffic_position[i][iii][2]) - tableData.startTime[0]) / singledeltaTime);//
						if (truecell < (BIGSUMMA + rgen) && (rgen != 1 && rsend != 1)) {//
							rgen = rgen - ((rgen + BIGSUMMA) - truecell);//
						}//
						BIGSUMMA += rgen;
						newFile << "\t\t<td title = '" << TG << tableData.traffic_position[i][iii][6] << "&#013;" << TS << tableData.traffic_position[i][iii][2] << "' class = '" << style1 << "' 	colspan = '" << to_string(rgen) << "' ></td>\n";
					}
					/*Отправка*/
					timetime = stof(tableData.traffic_position[i][iii][3]) - stof(tableData.traffic_position[i][iii][2]);
					CELL = timetime / singledeltaTime;
					rsend = int(ceil(CELL));
					if (rsend == 0) {
						rsend = 1;
					}
					int truecell = ceil((stof(tableData.traffic_position[i][iii][3]) - stof(tableData.traffic_position[i][iii][6])) / singledeltaTime);
					if (truecell < (rsend + rgen) && (rgen != 1 && rsend != 1)) {
						rsend = rsend - ((rsend + rgen) - truecell);
					}

					BIGSUMMA += rsend;
					if ((generationtraffics==1)||(generationtraffics==0)) {
						newFile << "\t\t<td title = '" << TS << tableData.traffic_position[i][iii][2] << "&#013;" << TE << tableData.traffic_position[i][iii][3] << "' class = '" << style2 + to_string((i + 1) % 21) << "' 	colspan = '" << to_string(rsend) << "' ></td>\n";
					}
					else {
						if (generationtraffics == 2) {
							newFile << "\t\t<td title = '" << TS << tableData.traffic_position[i][iii][2] << "&#013;" << TE << tableData.traffic_position[i][iii][3] << "' class = '" << style2 + to_string((i + 1) % 21) + style3 << "' 	colspan = '" << to_string(rsend) << "' ></td>\n";
						}
					}
					lasttime = stof(tableData.traffic_position[i][iii][3]);
				}
				else {/*Найден неправильный трафик, коотрый отправкой, генерацией или доставкой вышел за тайм-слот*/
						False_traffic.push_back(tableData.traffic_position[i][iii]);
						if (!((stof(tableData.traffic_position[i][iii][6]) <= (tableData.startTime[tableData.timeSlots - 1] + deltaTime))
							&& (stof(tableData.traffic_position[i][iii][3]) <= (tableData.startTime[tableData.timeSlots - 1] + deltaTime))
							&& (stof(tableData.traffic_position[i][iii][2]) <= (tableData.startTime[tableData.timeSlots - 1] + deltaTime))))
						{
							False_traffic[index].push_back("Выход пакета за эпоху");
						}

						if (!(stof(tableData.traffic_position[i][iii][2]) >= lasttime)
							&& (stof(tableData.traffic_position[i][iii][3]) >= lasttime))
						{
							False_traffic[index].push_back("Наложение пакетов друг на друга");
						}

						if (!(stof(tableData.traffic_position[i][iii][3]) >= stof(tableData.traffic_position[i][iii][2]))) 
						{
							False_traffic[index].push_back("Доставка < Отправки пакета");
						}
						if (!(stof(tableData.traffic_position[i][iii][6]) <= stof(tableData.traffic_position[i][iii][2]))) 
						{
							False_traffic[index].push_back("Генерация > Отправки пакета");
						}
						if (!(stof(tableData.traffic_position[i][iii][6]) <= stof(tableData.traffic_position[i][iii][3]))) 
						{
							False_traffic[index].push_back("Генерация > Доставки пакета");
						}
						False_Node.push_back(tableData.node[i]);
						index++;
						
						/*Добавить номер строки для вывода в traffic)pozition*/
				}
			}
			/*Дополнить пустотой конец*/
			int g = BIGSUMMA % number_of_cells;
			int rr = number_of_cells - g;
			newFile << "\t\t<td class = 'Empty' colspan = '" << to_string(rr) << "'></td>\n";// - оставшаяся пустота одиночек, не по 15
			BIGSUMMA += rr;
			for (int ii = BIGSUMMA / number_of_cells; ii < tableData.startTime.size(); ii++) {
				newFile << "\t\t<td class = 'Empty' colspan = '" << to_string(number_of_cells) << "'></td>\n";// - целая пустота ячеек пустота
			}

			newFile << "\t</tr>\n";
		}
	}
	newFile << "</tbody>\n";
	/*Конец вывода тел циклом для Hide по ID трафикам: Конец*/

	/*Вывод Шкалы времени*/

	newFile << "\n<tfood>\n<tr>\n";
	newFile << "\t<th colspan='2'>Время, мкс</th>\n";
	float time = 0;
	vector <string> n(1);
	for (int i = 0; i < tableData.timeSlots; i++) {
		n[0]= to_string(time);

		n[0] =n[0].substr(0, n[0].find('.', 0) + for_timeslots_new);
		int l = n[0].length();
		if (n[0].length() < length_for_time) {
			for (int i = 0; i != length_for_time - l; i++) {
				n[0].push_back('_');
			}
		}
		newFile << "\n\t\t<td colspan = '" << to_string(Number_of_divisions_in_time_for_one) << "'> <p class = 'vertical' class = 'time_begin'>" << n[0] << "</p></td>\n";


		time = time + deltaTime / Number_of_divisions_in_time;
		string s = to_string(int(time));
		l = s.length();
		for (int j = 0; j < Number_of_divisions_in_time - 1; j++) {
			if (s.length() < length_for_time) {
				for (int i = 0; i != length_for_time - l; i++) {
					s.push_back('_');
				}
			}
			newFile << "\t\t<td colspan = '" << to_string(Number_of_divisions_in_time_for_one) << "'> <p class = 'vertical' class = 'time'>" << s << "</p></td>\n";
			time = time + deltaTime / Number_of_divisions_in_time;
		}
	}
	newFile << "\t</tr>\n</tfoot>\n</table>\n";

	if (False_traffic.size() > 0) {
		newFile << "\n<table border='1' cellspacing='0'>\n\t<thead>\n\t<tr><th colspan = '7'>Обнаружены следующие ошибки при отображении пакетов</th> \n\t</tr>\n\t<tr>\n\t\t<th> Строка</th>\n\t\t<th> ID узла</th>\n\t\t<th> ID трафика</th>\n\t\t<th> Время генерации</th>\n\t\t<th> Время отправки</th>\n\t\t<th> Время доставки</th>\n\t\t<th> Причина</th>\n\t</tr>\n\t</thead>\n";
	}
	for (int i = 0; i < False_traffic.size(); i++) {
		newFile << "\t<tr>";
		newFile << "\n\t\t<th>" << False_traffic[i][7] << "</th>";
		newFile << "\n\t\t<th>" << False_Node[i] << "</th>";
		newFile << "\n\t\t<th>" << False_traffic[i][0] << "</th>";
		newFile << "\n\t\t<th>" << False_traffic[i][6] << "</th>";
		newFile << "\n\t\t<th>" << False_traffic[i][2] << "</th>";
		newFile << "\n\t\t<th>" << False_traffic[i][3] << "</th>";
		newFile << "\n\t\t<th>" << False_traffic[i][8] << "</th>";
		newFile << "\n\t</tr>";
	}
	newFile << "\n</table>";

	newFile << "<table>\n\t<tr>\n\t\t<th>Для смены цвета расписания используйте палитру<label for = 'maga'></label>\n\t\t\t<input type = 'checkbox' name = 'maga' id = 'maga' data-toggle = 'toggle'>\n\t\t</th>\n\t</tr>\n";
	newFile << "<tbody class = 'hide'>";







	//newFile << "\n<table>\n\t<tr>\n\t\t<td>" << tableData.node[0] << "</td>\n";
	//newFile << "\t\t<td> <input id ='1' type='radio' name='radio' class='active'></td>\n\t<tr>";
	for (int i = 0; i < tableData.node.size()/*Количество Узлов*/; i++) {
		newFile << "\n\t\t<tr>\n\t\t\t<td>" << tableData.node[i] << "</td>\n";
		newFile << "\t\t\t<td> <input id ='" << to_string(i + 1) << "' type='radio' name='radio'></td>\n\t\t</tr>";
	}
	newFile << "\t<td>\n";
	newFile << "\t\t<div id='color-picker' class='cp-default'>\n";
	newFile << "\t\t\t<div class='picker-wrapper'>\n";
	newFile << "\t\t\t\t<div id = 'picker' class = 'picker'></div>\n";
	newFile << "\t\t\t\t<div id = 'picker-indicator' class = 'picker-indicator'></div>\n\t\t\t</div>\n";
	newFile << "\t\t\t<div class = 'pcr-wrapper'>\n\t\t\t\t<div id = 'pcr' class = 'pcr'></div>\n\t\t\t\t<div id = 'pcr-indicator' class = 'pcr-indicator'></div>\n\t\t\t</div>\n";
	newFile << "\t\t\t<ul id='color-values'>\n\t\t\t\t<li><label>RGB:</label><span id = 'rgb'></span></li>\n\t\t\t\t<li><label>HSV:</label><span id = 'hsv'></span></li>\n";
	newFile << "\t\t\t\t<li><label>HEX:</label><span id = 'hex'></span></li>\n\t\t\t\t<li><div id = 'pcr_bg'></div></li>\n\t\t\t</ul>\n";
	newFile << "\t\t</div>\n<script src = 'new.js'></script>\n</tbody>\n</table>";
}

void Colorpickermin() {
	ofstream color("colorpicker.min.js");
	color << "(function(h,k,j){function r(a){if(h.event&&h.event.contentOverflow!==j)return{x:h.event.offsetX,y:h.event.offsetY};if(a.offsetX!==j&&a.offsetY!==j)return{x:a.offsetX,y:a.offsetY};var b=a.target.parentNode.parentNode;return{x:a.layerX-b.offsetLeft,y:a.layerY-b.offsetTop}}function e(a,b,c){a=k.createElementNS(v,a);for(var d in b)a.setAttribute(d,b[d]);if(Object.prototype.toString.call(c)!=\"[object Array]\")c=[c];b=0;for(d=c[0]&&c.length||0;b<d;b++)a.appendChild(c[b]);return a}function l(a,b,c){var d," << endl;
		color << "f,g;a=a%360/60;g=c*b;f=g*(1-Math.abs(a%2-1));d=b=c-=g;a=~~a;d+=[g,f,0,0,f,g][a];b+=[f,g,g,f,0,0][a];c+=[0,0,f,g,g,f][a];a=d*255;b*=255;c*=255;return{r:a,g:b,b:c,hex:\"#\"+(16777216|c|b<<8|a<<16).toString(16).slice(1)}}function s(a,b,c){if(a>1||b>1||c>1){a/=255;b/=255;c/=255}var d,f;d=Math.max(a,b,c);f=d-Math.min(a,b,c);return{h:(f==0?null:d==a?(b-c)/f:d==b?(c-a)/f+2:(a-b)/f+4)%6*60,s:f==0?0:f/d,v:d}}function t(a,b,c){return function(d){d=d||h.event;d=r(d);a.h=d.y/b.offsetHeight*360+o;var f=l(a.h,1," << endl;
		color << "1);c.style.backgroundColor=f.hex;a.callback&&a.callback(f.hex,{h:a.h-o,s:a.s,v:a.v},{r:f.r,g:f.g,b:f.b},j,d)}}function u(a,b){return function(c){c=c||h.event;c=r(c);var d=b.offsetHeight;a.s=c.x/b.offsetWidth;a.v=(d-c.y)/d;d=l(a.h,a.s,a.v);a.callback&&a.callback(d.hex,{h:a.h-o,s:a.s,v:a.v},{r:d.r,g:d.g,b:d.b},c)}}function i(a,b,c){if(!(this instanceof i))return new i(a,b,c);this.callback=c;this.h=0;this.v=this.s=1;this.pickerElement=b;this.slideElement=a;if(p==\"SVG\"){a.appendChild(m.cloneNode(true));" << endl;
		color << "b.appendChild(n.cloneNode(true))}else{a.innerHTML=m;b.innerHTML=n}if(a.attachEvent){a.attachEvent(\"onclick\",t(this,a,b));b.attachEvent(\"onclick\",u(this,b))}else if(a.addEventListener){a.addEventListener(\"click\",t(this,a,b),false);b.addEventListener(\"click\",u(this,b),false)}}function q(a,b,c,d){a.h=b.h%360;a.s=b.s;a.v=b.v;b=l(a.h,a.s,a.v);var f={y:a.h*a.slideElement.offsetHeight/360,x:0},g=a.pickerElement.offsetHeight;g={x:a.s*a.pickerElement.offsetWidth,y:g-a.v*g};a.pickerElement.style.backgroundColor=" << endl;
		color << "l(a.h,1,1).hex;a.callback&&a.callback(d||b.hex,{h:a.h,s:a.s,v:a.v},c||{r:b.r,g:b.g,b:b.b},g,f)}var p=h.SVGAngle||k.implementation.hasFeature(\"http://www.w3.org/TR/SVG11/feature#BasicStructure\",\"1.1\")?\"SVG\":\"VML\",n,m,o=15,v=\"http://www.w3.org/2000/svg\";if(p==\"SVG\"){m=e(\"svg\",{xmlns:\"http://www.w3.org/2000/svg\",version:\"1.1\",width:\"100%\",height:\"100%\"},[e(\"defs\",{},e(\"linearGradient\",{id:\"gradient-hsv\",x1:\"0%\",y1:\"100%\",x2:\"0%\",y2:\"0%\"},[e(\"stop\",{offset:\"0%\",\"stop-color\":\"#FF0000\",\"stop-opacity\":\"1\"})," << endl;
		color << "e(\"stop\",{offset:\"13%\",\"stop-color\":\"#FF00FF\",\"stop-opacity\":\"1\"}),e(\"stop\",{offset:\"25%\",\"stop-color\":\"#8000FF\",\"stop-opacity\":\"1\"}),e(\"stop\",{offset:\"38%\",\"stop-color\":\"#0040FF\",\"stop-opacity\":\"1\"}),e(\"stop\",{offset:\"50%\",\"stop-color\":\"#00FFFF\",\"stop-opacity\":\"1\"}),e(\"stop\",{offset:\"63%\",\"stop-color\":\"#00FF40\",\"stop-opacity\":\"1\"}),e(\"stop\",{offset:\"75%\",\"stop-color\":\"#0BED00\",\"stop-opacity\":\"1\"}),e(\"stop\",{offset:\"88%\",\"stop-color\":\"#FFFF00\",\"stop-opacity\":\"1\"}),e(\"stop\",{offset:\"100%\",\"stop-color\":\"#FF0000\"," << endl;
		color << "\"stop-opacity\":\"1\"})])),e(\"rect\",{x:\"0\",y:\"0\",width:\"100%\",height:\"100%\",fill:\"url(#gradient-hsv)\"})]);n=e(\"svg\",{xmlns:\"http://www.w3.org/2000/svg\",version:\"1.1\",width:\"100%\",height:\"100%\"},[e(\"defs\",{},[e(\"linearGradient\",{id:\"gradient-black\",x1:\"0%\",y1:\"100%\",x2:\"0%\",y2:\"0%\"},[e(\"stop\",{offset:\"0%\",\"stop-color\":\"#000000\",\"stop-opacity\":\"1\"}),e(\"stop\",{offset:\"100%\",\"stop-color\":\"#CC9A81\",\"stop-opacity\":\"0\"})]),e(\"linearGradient\",{id:\"gradient-white\",x1:\"0%\",y1:\"100%\",x2:\"100%\",y2:\"100%\"},[e(\"stop\"," << endl;
		color << "{offset:\"0%\",\"stop-color\":\"#FFFFFF\",\"stop-opacity\":\"1\"}),e(\"stop\",{offset:\"100%\",\"stop-color\":\"#CC9A81\",\"stop-opacity\":\"0\"})])]),e(\"rect\",{x:\"0\",y:\"0\",width:\"100%\",height:\"100%\",fill:\"url(#gradient-white)\"}),e(\"rect\",{x:\"0\",y:\"0\",width:\"100%\",height:\"100%\",fill:\"url(#gradient-black)\"})])}else if(p==\"VML\"){m='<DIV style=\"position: relative; width: 100%; height: 100%\"><v:rect style=\"position: absolute; top: 0; left: 0; width: 100%; height: 100%\" stroked=\"f\" filled=\"t\"><v:fill type=\"gradient\" method=\"none\" angle=\"0\" color=\"red\" color2=\"red\" colors=\"8519f fuchsia;.25 #8000ff;24903f #0040ff;.5 aqua;41287f #00ff40;.75 #0bed00;57671f yellow\"></v:fill></v:rect></DIV>';" << endl;
		color << "n='<DIV style=\"position: relative; width: 100%; height: 100%\"><v:rect style=\"position: absolute; left: -1px; top: -1px; width: 101%; height: 101%\" stroked=\"f\" filled=\"t\"><v:fill type=\"gradient\" method=\"none\" angle=\"270\" color=\"#FFFFFF\" opacity=\"100%\" color2=\"#CC9A81\" o:opacity2=\"0%\"></v:fill></v:rect><v:rect style=\"position: absolute; left: 0px; top: 0px; width: 100%; height: 101%\" stroked=\"f\" filled=\"t\"><v:fill type=\"gradient\" method=\"none\" angle=\"0\" color=\"#000000\" opacity=\"100%\" color2=\"#CC9A81\" o:opacity2=\"0%\"></v:fill></v:rect></DIV>';" << endl;
		color << "k.namespaces.v||k.namespaces.add(\"v\",\"urn:schemas-microsoft-com:vml\",\"#default#VML\")}i.prototype.setHsv=function(a){q(this,a)};i.prototype.setRgb=function(a){q(this,s(a.r,a.g,a.b),a)};i.prototype.setHex=function(a){q(this,s(parseInt(a.substr(1,2),16),parseInt(a.substr(3,2),16),parseInt(a.substr(5,2),16)),j,a)};i.positionIndicators=function(a,b,c,d){if(c){b.style.left=\"auto\";b.style.right=\"0px\";b.style.top=\"0px\";a.style.top=c.y-a.offsetHeight/2+\"px\"}if(d){b.style.top=d.y-b.offsetHeight/2+\"px\";b.style.left=" << endl;
		color << "d.x-b.offsetWidth/2+\"px\"}};h.ColorPicker=i})(window,window.document);" << endl;

}

void WorktoColor() {
	ofstream work("worktocolor.js");
	work << "var nameBusy = \".Busy1\";" << endl
		<< "cp = ColorPicker(" << endl
		<< "document.getElementById('pcr')," << endl
		<< "document.getElementById('picker')," << endl
		<< "function(hex, hsv, rgb, mousePicker, mousepcr) {" << endl
		<< "currentColor = hex;" << endl
		<< "ColorPicker.positionIndicators(document.getElementById('pcr-indicator'), document.getElementById('picker-indicator'), mousepcr, mousePicker);" << endl
		<< "document.getElementById('hex').innerHTML = hex;" << endl
		<< "document.getElementById('rgb').innerHTML = 'rgb(' + rgb.r.toFixed() + ',' + rgb.g.toFixed() + ',' + rgb.b.toFixed() + ')';" << endl
		<< "document.getElementById('hsv').innerHTML = 'hsv(' + hsv.h.toFixed() + ',' + hsv.s.toFixed(2) + ',' + hsv.v.toFixed(2) + ')';\n" << endl

		<< "document.getElementById('pcr_bg').style.backgroundColor = hex;" << endl
		<< "var matches = document.querySelectorAll(nameBusy);" << endl
		<< "for (var i = 0; i < matches.length; i++) {" << endl
		<< "matches[i].style.backgroundColor = hex;" << endl
		<< "}" << endl
		<< "}" << endl
		<< ");" << endl
		<< "$('input[name=\"radio\"]').click(function()" << endl
		<< "{" << endl
		<< "$('.active').removeClass('active');" << endl
		<< "$(this).addClass('active');" << endl
		<< "let id = $(this).attr('id');" << endl
		<< "nameBusy = \".Busy\" + id;" << endl
		<< "})\n" << endl

		<< "cp.setHex('#D4EDFB');" << endl
		<< "var $active = $('.active').attr(\"checked\",\"checked\");" << endl;
}


void copyJSfiles(string src_dir, string dest_dir, string file_name) {
    string src = "\"";  src += src_dir;  src += file_name;  src += "\"";
    string dest = "\""; dest += dest_dir; dest += file_name; dest += "\"";

    string command;
    command.append("copy ");
    command.append(src);
    command.append(" ");
    command.append(dest);
    string c = "@echo ";
    c += command.c_str();

    system(c.c_str());
    system(command.c_str());
}


string getDir(string path) {
    std::string::size_type n = path.rfind("\\");
    string dir;
    if (n != std::string::npos)
        dir = path.substr(0, n + 1);
    else
        dir = ".\\";
    return dir;
}


void main(int argc, char* argv[])
{
	setlocale(LC_CTYPE, "rus");

    string inputpath;
    if (argc > 1)
        inputpath = argv[1];
    else
        //inputpath = "C:\\Users\\User\\AppData\\Local\\VIPE\\scheduler_output.xml";
        //inputpath = "C:\\project\\CADS\\comp3\\xml_html_converter\\xml\\scheduler_output.xml";
        inputpath = ".\\xml\\scheduler_output2.xml";

    string outputpath;
    if (argc > 2)
        outputpath = argv[2];
    else
        //outputpath = "C:\\Users\\User\\AppData\\Local\\VIPE\\scheduler_output.html";
        //outputpath = "C:\\project\\CADS\\comp3\\xml_html_converter\\xml\\scheduler_output.html";
        outputpath = ".\\html\\scheduler_output2.html";

    if (argc > 3) {
        Number_of_divisions_in_time = atoi(argv[3]);
        number_of_cells = Number_of_divisions_in_time*Number_of_divisions_in_time_for_one;
    }



    string srcdir = getDir(argv[0]);
    string destdir = getDir(outputpath);

    
    
    copyJSfiles(srcdir, destdir, "new.js");
    copyJSfiles(srcdir, destdir, "jquery-3.4.1.min.js");
    copyJSfiles(srcdir, destdir, "colorpicker.min.js");

    string jspath = "C:\\project\\CADS\\comp3\\xml_html_converter\\js_libs\\";

    Style2(destdir);
    OpeningJS(destdir);
    XMLfile Load(inputpath);
    int res = Load.convert(outputpath, jspath);

    ofstream log;
    log.open("Fractal_log.txt");
    log << "inputpath =  " << inputpath << endl;
    log << "outputpath = " << outputpath << endl;
    log << "Number_of_divisions_in_time = " << Number_of_divisions_in_time << endl;

    if (res == -1)
        log << "NO FILE ACCESS" << endl;
    else
        log << "HTML was read";
    log.close();

	return ;
}