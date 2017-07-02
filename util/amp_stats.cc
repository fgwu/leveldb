#include <math.h>
#include <stdio.h>
#include "port/port.h"
#include "util/amp_stats.h"

namespace leveldb {

AmpStats::AmpStats():
  read_lat_mem_(0),
  read_lat_mem_cnt_(0),
  read_lat_imm_(0),
  read_lat_imm_cnt_(0),
  read_lat_tbl_(0),
  read_lat_tbl_cnt_(0),
  read_lat_cache_(0),
  read_lat_cache_cnt_(0),
  read_lat_disk_(0),
  read_lat_disk_cnt_(0)
{ 
  fprintf(stdout, "AmpStats init\n");
  for (int i = 0; i < kNumReadCntLimit; i++) {
    read_cnt_bucket_[i] = 0;
    read_lat_bucket_[i] = 0;
  }
}

AmpStats::~AmpStats() {
  fprintf(stdout, "AmpStats destroy\n");
}

std::string AmpStats::ToString() const {
  std::string s = "";
  char buf[200];
  snprintf(buf, sizeof(buf), "AmpStats:\nRead Lat Mem Avg: %.3f Cnt: %ld\n",
	   read_lat_mem_/read_lat_mem_cnt_,
	   read_lat_mem_cnt_);
  s.append(buf);
  snprintf(buf, sizeof(buf), "Read Lat Imm Avg: %.3f Cnt: %ld\n",
	   read_lat_imm_/read_lat_imm_cnt_,
	   read_lat_imm_cnt_);
  s.append(buf);
  snprintf(buf, sizeof(buf), "Read Lat Tbl Avg: %.3f Cnt: %ld\n",
	   read_lat_tbl_/read_lat_tbl_cnt_,
	   read_lat_tbl_cnt_);
  s.append(buf);
  snprintf(buf, sizeof(buf), "Read Lat Cache Avg: %.3f Cnt: %ld\n",
	   read_lat_cache_/read_lat_cache_cnt_,
	   read_lat_cache_cnt_);
  s.append(buf);
  snprintf(buf, sizeof(buf), "Read Lat Disk Avg: %.3f Cnt: %ld\n",
	   read_lat_disk_/read_lat_disk_cnt_,
	   read_lat_disk_cnt_);
  s.append(buf);

  snprintf(buf, sizeof(buf), "Read File Cnt:\n");
  s.append(buf);

  long total_entry = 0;
  for (int i = 0; i < kNumReadCntLimit; i++) {
    snprintf(buf, sizeof(buf), "#sst read %7d: %7d %10.3f\n",
	     i, read_cnt_bucket_[i],
	     read_lat_bucket_[i]/read_cnt_bucket_[i]);
    total_entry += read_cnt_bucket_[i];
    s.append(buf);
  }

  snprintf(buf, sizeof(buf),   "total entrys:      %7ld\n",
	   total_entry);
  s.append(buf);


  return s;
}

void AmpStats::Add(AmpStats::Type t, double micros) {
  switch(t) {
  case kMem:
    read_lat_mem_ += micros;
    read_lat_mem_cnt_++;
    break;
  case kImm:
    read_lat_imm_ += micros;
    read_lat_imm_cnt_++;
    break;
  case kTbl:
    read_lat_tbl_ += micros;
    read_lat_tbl_cnt_++;
    break;
  case kCache:
    read_lat_cache_ += micros;
    read_lat_cache_cnt_++;
    break;
  case kDisk:
    read_lat_disk_ += micros;
    read_lat_disk_cnt_++;
    break;
  }
}

  void AmpStats::AddReadCntLat(int cnt, double micros) {
    if (cnt >= 0 && cnt < kNumReadCntLimit) {
      read_cnt_bucket_[cnt]++;
      read_lat_bucket_[cnt] += micros;
    }
  }

}  // namespace leveldb
