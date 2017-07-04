#ifndef STORAGE_LEVELDB_UTIL_AMP_STATS_H_
#define STORAGE_LEVELDB_UTIL_AMP_STATS_H_

namespace leveldb {
class AmpStats {
 public:
  AmpStats();
  ~AmpStats();
  std::string ToString() const;
  enum Type {kMem, kImm, kTbl, kCache, kDisk, kReadCnt};
  void AddReadLat(Type, double, int, int, bool);

 private:
  double read_lat_mem_;
  long read_lat_mem_cnt_;
  double read_lat_imm_;
  long read_lat_imm_cnt_;
  double read_lat_tbl_;
  long read_lat_tbl_cnt_;
  double read_lat_cache_;
  long read_lat_cache_cnt_;
  double read_lat_disk_;
  long read_lat_disk_cnt_;

  enum { kNumReadCntLimit = 30 };
  int sst_cnt_bucket_[kNumReadCntLimit];
  int sst_cnt_bucket_success_[kNumReadCntLimit];
  double sst_cnt_lat_bucket_[kNumReadCntLimit];

  enum { kNumSeekLevelLimit = 10 };
  int seek_level_bucket_[kNumSeekLevelLimit];
  int seek_level_bucket_success_[kNumSeekLevelLimit];
  double seek_level_lat_bucket_[kNumSeekLevelLimit];

  void AddType(Type, double);
  void AddReadCntLat(int, double, bool);
  void AddSeekLevelLat(int, double, bool);
};

} // namespace leveldb

#endif // STORAGE_LEVELDB_UTIL_AMP_STATS_H_
