#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <iostream>
#include <sstream>
#include <thread>
#include <mutex>

#include "webview.hpp"
#include "JobQueue.hpp"

#define LOG(mesg) std::cout << mesg << std::endl;

class Application
{

public:
    Application(/* args */)
    {
        m_hostView = new webview::webview(true, nullptr);
        m_hostView->set_title("Test Prog");
        m_hostView->set_size(480, 320, WEBVIEW_HINT_NONE);
        m_hostView->set_size(180, 120, WEBVIEW_HINT_MIN);
        m_hostView->set_update_callback([this](){
            this->OnUpdate();
        });

        InitAppInfo();        
        InitBinding();
    };

    void Run()
    {
        m_hostView->navigate(m_indexPath);
        m_hostView->run();
    }

    ~Application(){};

private:

    void InitBinding()
    {
        m_hostView->bind("noop", [](std::string seq, std::string arg) {
        });

        m_hostView->bind("add", [&](std::string seq, std::string arg) {
            m_jobQueue.PushJob([&, seq](){
                std::this_thread::sleep_for(std::chrono::seconds(3));
                LOG(" ========== after 3s =========");
                m_hostView->resolve(seq, 0, std::string("\"fsdf sf  fsd \""));
            });
        });
    }

    void InitAppInfo()
    {
        char pBuf[256];
        size_t len = sizeof(pBuf);
        int bytes = MIN(readlink("/proc/self/exe", pBuf, len), len - 1);
        pBuf[bytes] = '\0';

        std::string curDir = std::string(pBuf);
        curDir = std::string(curDir.begin(), curDir.begin() + curDir.find_last_of('/'));

        m_assetsPath = curDir + "/assets/";

        std::stringstream sFileIndex;
        sFileIndex << "file://" << curDir << "/assets/index.html";

        m_indexPath = sFileIndex.str();
    }

    void OnUpdate()
    {
        // std::unique_lock<std::mutex> lock(m_mutex);
        // if(!m_listJsCallbackResolve.empty())
        // {
        //     m_hostView->eval("testEval()");
        //     m_listJsCallbackResolve.pop_back();
        // }
    }

private:
    webview::webview   *m_hostView;
    static Application *s_instance;

    std::mutex    m_mutex;
    JobQueue      m_jobQueue;
    std::vector<int> m_listJsCallbackResolve;

    std::string m_assetsPath = "";
    std::string m_indexPath  = "";
};
#endif