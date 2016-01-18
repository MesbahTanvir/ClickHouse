#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>

#include <DB/Common/ProfileEvents.h>


namespace DB
{

/**	Automatically sends difference of ProfileEvents to Graphite at beginning of every minute
  */
class MetricsTransmitter
{
public:
	~MetricsTransmitter();

private:
	void run();
	void transmitCounters();

	/// Значения счётчиков при предыдущей отправке (или нули, если ни разу не отправляли).
	decltype(ProfileEvents::counters) prev_counters{};

	bool quit = false;
	std::mutex mutex;
	std::condition_variable cond;
	std::thread thread {&MetricsTransmitter::run, this};

	static constexpr auto event_path_prefix = "ClickHouse.ProfileEvents.";
};

}
