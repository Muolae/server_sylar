#include "log.h"
#include <memory>
#include <string>
#include <stdint.h>
#include <tuple>

namespace sylar {
	void sayhello() {
		cout << "helloworld" << endl;
	}

	Logger::Logger(const string& name)
		:m_name(name) {};
	void Logger::log(LogLevel::Level level, LogEvent::ptr event) {
		if (level > m_level) {
			//启动所有的输出器输出日志
			for (auto i : m_appends)i->log(level, event);
		}
	};
	void Logger::addLogAppender(LogAppender::ptr append) {
		m_appends.push_back(append);
	}
	void Logger::delLogAppender(LogAppender::ptr append) {
		for (auto i = m_appends.begin(); i != m_appends.end(); i++) {
			if (*i == append) {
				m_appends.erase(i);
				break;
			}
		}
	}
	void Logger::DEBUG(LogEvent::ptr event) {
		log(LogLevel::DEBUG, event);
	};
	void Logger::INFO(LogEvent::ptr event){
		log(LogLevel::INFO, event);
	};
	void Logger::WARN(LogEvent::ptr event){
		log(LogLevel::WARN, event);
	};
	void Logger::ERROR(LogEvent::ptr event) {
		log(LogLevel::ERROR, event);
	};
	void Logger::FATAL(LogEvent::ptr event) {
		log(LogLevel::FATAL, event);
	};
	void StdouAppender::log(LogLevel::Level level, LogEvent::ptr event) {
		if (level > m_level) {
			cout << format->format(event) << endl;
		}
	}
	FileAppender::FileAppender(const string& filename) 
		:m_filename(filename){

	}
	bool FileAppender::reopen() {
		if (m_filestream) {
			m_filestream.close();
		}
		m_filestream.open(m_filename);
		return !!m_filestream;
	}
	void FileAppender::log(LogLevel::Level level, LogEvent::ptr event) {
		if (level > m_level) {
			m_filestream << format->format(event);
		}
	}
	void LogFormatter::init() {
		vector<tuple<string,string, int> >vec;
		string str;
		for (size_t i = 0; i < m_pattern.size(); i++) {
			if (m_pattern[i] != '%') {
				str.append(1, m_pattern[i]);
				continue;
			}
			size_t n = i + 1;
			int fmt_status = 0;
			size_t fmt_begin = 0;
			string str, fmt;
			while (n < m_pattern.size()) {
				if (isspace(m_pattern[n])) {
					break;
				}
				if (fmt_status == 0) {
					if (m_pattern[n] == '{') {
						str = m_pattern.substr(i + 1, n - i - 1);
						fmt_status = 1; //解析格式
						fmt_begin = n;
						++n;
						continue;
					}
				}
				else if (fmt_status == 1) {
					if (m_pattern[n] == '}') {
						fmt = m_pattern.substr(fmt_begin + 1, n - fmt_begin - 1);
						//std::cout << "#" << fmt << std::endl;
						fmt_status = 2;
						++n;
						continue;
					}
				}
			}
			if (fmt_status == 0) {
				str = m_pattern.substr(i + 1, n - i + 1);
				//vec.push_back(std::make_tuple(str, std::string(), 0));
				/*nstr.clear();*/
				
				vec.push_back(std::make_tuple(str, fmt, 1));
				i = n ;
			}
			else if (fmt_status == 1) {
				std::cout << "pattern parse error: " << m_pattern << " - " << m_pattern.substr(i) << std::endl;
				//m_error = true;
				vec.push_back(std::make_tuple("<<pattern_error>>", fmt, 0));
			}
			else if (fmt_status == 2) {
				vec.push_back(make_tuple(str, fmt, 1));
				i = n;
			}
		}
	}
	string LogFormatter::format(LogEvent::ptr event){
		stringstream ss;
		for (auto i : m_items) {
			i->format(ss, event);
		}
		return ss.str();
	}
	LogFormatter::LogFormatter(const string& pattern)
	:m_pattern(pattern){

	}
	
}
