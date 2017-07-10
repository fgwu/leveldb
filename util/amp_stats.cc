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
    sst_cnt_bucket_[i] = 0;
    sst_cnt_bucket_success_[i] = 0;
    sst_cnt_lat_bucket_[i] = 0;
  }

  for (int i = 0; i < kNumSeekLevelLimit; i++) {
    seek_level_bucket_[i] = 0;
    seek_level_bucket_success_[i] = 0;
    seek_level_lat_bucket_[i] = 0;
  }
}

AmpStats::~AmpStats() {
  fprintf(stdout, "AmpStats destroy\n");
}

std::string AmpStats::ToString() const {
  std::string s = "";
  char buf[200];
  snprintf(buf, sizeof(buf), "AmpStats:\nRead Lat Mem   Avg: %7.3f Cnt: %12ld\n",
	   read_lat_mem_/read_lat_mem_cnt_,
	   read_lat_mem_cnt_);
  s.append(buf);
  snprintf(buf, sizeof(buf), "Read Lat Imm   Avg: %7.3f Cnt: %12ld\n",
	   read_lat_imm_/read_lat_imm_cnt_,
	   read_lat_imm_cnt_);
  s.append(buf);
  snprintf(buf, sizeof(buf), "Read Lat Tbl   Avg: %7.3f Cnt: %12ld\n",
	   read_lat_tbl_/read_lat_tbl_cnt_,
	   read_lat_tbl_cnt_);
  s.append(buf);
  snprintf(buf, sizeof(buf), "Read Lat Cache Avg: %7.3f Cnt: %12ld\n",
	   read_lat_cache_/read_lat_cache_cnt_,
	   read_lat_cache_cnt_);
  s.append(buf);
  snprintf(buf, sizeof(buf), "Read Lat Disk  Avg: %7.3f Cnt: %12ld\n",
	   read_lat_disk_/read_lat_disk_cnt_,
	   read_lat_disk_cnt_);
  s.append(buf);

  snprintf(buf, sizeof(buf), "\n\nRead File Cnt:\n");
  s.append(buf);

  long total_entry = 0;
  long total_found = 0;

  snprintf(buf, sizeof(buf), "          %7s %7s %7s %7s %5s %10s\n"
	     "----------------------------------------------------------\n",
	   "", "found", "miss", "total", "miss%",  "avg_lat");
  s.append(buf);
  for (int i = 0; i < kNumReadCntLimit; i++) {
    int miss = sst_cnt_bucket_[i] - sst_cnt_bucket_success_[i];
    snprintf(buf, sizeof(buf), "sst cnt   %7d:%7d %7d %7d %5.1f %10.3f\n",
	     i, sst_cnt_bucket_success_[i], 
	     miss,
	     sst_cnt_bucket_[i],
	     miss * 100.0/sst_cnt_bucket_[i],
	     sst_cnt_lat_bucket_[i]/sst_cnt_bucket_[i]);
    total_entry += sst_cnt_bucket_[i];
    total_found += sst_cnt_bucket_success_[i];
    s.append(buf);
  }
  snprintf(buf, sizeof(buf),   "total entries:    %7ld %7ld %7ld %5.1f\n",
	   total_found, 
	   total_entry - total_found,
	   total_entry,
	   (total_entry - total_found) * 100.0/total_entry);
  s.append(buf);


  total_entry = 0;
  total_found = 0;
  snprintf(buf, sizeof(buf), "\n\n          %7s %7s %7s %7s %5s %10s\n"
	     "----------------------------------------------------------\n",
	   "", "found", "miss", "total", "miss%", "avg_lat");
  s.append(buf);
  for (int i = 0; i < kNumSeekLevelLimit; i++) {
    int miss = seek_level_bucket_[i] - 
      seek_level_bucket_success_[i];
    snprintf(buf, sizeof(buf), "seek level%7d:%7d %7d %7d %5.1f %10.3f\n",
	     i - 1, 
	     seek_level_bucket_success_[i],
	     miss,
	     seek_level_bucket_[i],
	     miss * 100.0/seek_level_bucket_[i],
	     seek_level_lat_bucket_[i]/seek_level_bucket_[i]);
    total_entry += seek_level_bucket_[i];
    total_found += seek_level_bucket_success_[i];
    s.append(buf);
  }
  snprintf(buf, sizeof(buf),   "total entries:    %7ld %7ld %7ld %5.1f\n",
	   total_found, 
	   total_entry - total_found,
	   total_entry,
	   (total_entry - total_found) * 100.0/total_entry);
  s.append(buf);


  return s;
}

void AmpStats::AddType(AmpStats::Type t, double micros) {
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

void AmpStats::AddReadCntLat(int cnt, 
			     double micros, bool found) {
    sst_cnt_bucket_[cnt]++;
    if (found) sst_cnt_bucket_success_[cnt]++;
    sst_cnt_lat_bucket_[cnt] += micros;
}

void AmpStats::AddSeekLevelLat(int seek_level, 
			       double micros, bool found) {
  // Note that seek_level starts from -1.
  // so we shift the bucket by 1. 
  // when prining out, we shift back. See ToString();
    seek_level_bucket_[seek_level + 1]++;
    if (found) seek_level_bucket_success_[seek_level + 1]++;
    seek_level_lat_bucket_[seek_level + 1] += micros;
}

void AmpStats::AddReadLat(double micros, 
			  int cnt, int seek_level, bool found) {
  AddReadCntLat(cnt, micros, found);
  AddSeekLevelLat(seek_level, micros, found);
}

}  // namespace leveldb
