#pragma once
#include <iostream>
#include <list>
#include <sstream>
#include <fstream>
#include <vector>
using namespace std;

namespace sylar {

	void sayhello ();

	//��־�¼�
	class LogEvent {
	public:
		typedef shared_ptr<LogEvent> ptr;
		LogEvent();
	private:
		const char* fliename=nullptr;//�ļ���
		int32_t m_line=0;//�к�
		uint32_t m_elapse=0;//����ʱ��
		uint32_t threadid=0;//�߳�id
		uint32_t fiberid=0;//Э��id
		uint64_t m_time=0;//ʱ���
		string m_content=0;//��־�������
	};

	//��־����
	class LogLevel {
	public:
		enum Level {
			DEBUG=1,
			INFO,
			WARN,
			ERROR,
			FATAL
		};
	};

	//��־��ʽ��
	class LogFormatter {
	public:
		typedef shared_ptr<LogFormatter> ptr;
		string format(LogEvent::ptr event);
		LogFormatter(const string& pattern);
	private:
		class FormatItem {
		public:
			typedef shared_ptr<FormatItem> ptr;
			virtual ~FormatItem();
			virtual void format(ostream& os,LogEvent::ptr event)=0;
		};
	private:
		vector<FormatItem::ptr>m_items;
		string m_pattern;
		void init();
		
	};

	//��־�����
	class LogAppender {
	public:
		typedef shared_ptr<LogAppender> ptr;
		virtual ~LogAppender();

		virtual void log(LogLevel::Level level,LogEvent::ptr event)=0;

		void setFormat(LogFormatter::ptr val) { format = val; };
		LogFormatter::ptr getFormat() { return format; };
	protected:
		LogLevel::Level m_level;
		LogFormatter::ptr format;
	};

	//��־��
	class Logger {
	public:
		typedef shared_ptr<Logger> ptr;
		Logger(const string& name = "root");
		void log(LogLevel::Level level, LogEvent::ptr event);

		void DEBUG(LogEvent::ptr event);
		void INFO (LogEvent::ptr event);
		void WARN (LogEvent::ptr event);
		void ERROR(LogEvent::ptr event);
		void FATAL(LogEvent::ptr event);

		void addLogAppender(LogAppender::ptr append);
		void delLogAppender(LogAppender::ptr append);

		LogLevel::Level getLevel() { return m_level; };
		void setLevel(LogLevel::Level level) { this->m_level = level; }
	private:
		LogLevel::Level m_level;
		list<LogAppender::ptr> m_appends;//append����
		string m_name;
	};

	//���������̨��append
	class StdouAppender :public LogAppender {
	public:
		typedef shared_ptr<StdouAppender> ptr;
		virtual void log (LogLevel::Level level, LogEvent::ptr event)override;
	};

	//������ļ���append
	class FileAppender :public LogAppender {
	public:
		typedef shared_ptr<FileAppender>ptr;
		FileAppender(const string& filename);
		virtual void log(LogLevel::Level level, LogEvent::ptr event) override;
		//���´��ļ����򿪳ɹ�����true
		bool reopen();
	private:
		string m_filename;
		ofstream m_filestream;
	};
}
