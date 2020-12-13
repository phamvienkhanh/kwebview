#ifndef JOBQUEUE_HPP
#define JOBQUEUE_HPP

#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include <functional>
#include <condition_variable>

using Job = std::function<void(void)>;

class JobQueue
{
public:

    JobQueue(/* args */)
    {
        for(int i = 0; i < m_numWorkers; i++)
        {
            std::thread *_thread = new std::thread([&]() {
                std::unique_lock<std::mutex> lock(m_mutex);

                do
                {
                    m_conditionVar.wait(lock, [this]() {
                        return (m_jobQueue.size() || m_isStop);
                    });

                    if (m_jobQueue.size())
                    {
                        auto job = m_jobQueue.front();
                        m_jobQueue.pop();

                        lock.unlock();

                        if (job != nullptr)
                            job();

                        lock.lock();
                    }
                } while (!m_isStop);
            });
            m_workers.push_back(_thread);
            _thread->detach();
        }
    };

    void PushJob(Job job)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_jobQueue.push(job);
        lock.unlock();
        m_conditionVar.notify_one();
    }

    ~JobQueue()
    {
        m_isStop = true;
        m_mutex.unlock();
        m_conditionVar.notify_all();

        for(int i = 0; i < m_numWorkers; i++)
        {
            if(m_workers[i]->joinable())
                m_workers[i]->join();
        }
    };

private:
    int m_numWorkers = 2;
    bool m_isStop = false;
    std::mutex m_mutex;
    std::condition_variable  m_conditionVar;
    std::vector<std::thread*> m_workers;
    std::queue<Job> m_jobQueue;
};
#endif
