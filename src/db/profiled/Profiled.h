/********************************************//**
 * Copyright @ Members of the EMI Collaboration, 2010.
 * See www.eu-emi.eu for details on the copyright holders.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ***********************************************/

#pragma once

#include <common_dev.h>
#include <iomanip>
#include <map>
#include "GenericDbIfce.h"

/**
 * This class is intended to wrap implementations of the DB,
 * profiling their times.
 */
class ProfiledDB : public GenericDbIfce
{
private:
    GenericDbIfce *db;
    void (*destroy_db)(void *);

public:
    ProfiledDB(GenericDbIfce* db, void (*destroy_db)(void *));
    ~ProfiledDB();

    void init(std::string username, std::string password, std::string connectString, int pooledConn);

    void submitPhysical(const std::string & jobId, std::list<job_element_tupple> src_dest_pair,
                        const std::string & DN, const std::string & cred,
                        const std::string & voName, const std::string & myProxyServer, const std::string & delegationID,
                        const std::string & sourceSe, const std::string & destinationSe,
                        const JobParameterHandler & params);

    void getTransferJobStatus(std::string requestID, bool archive, std::vector<JobStatus*>& jobs);

    void getTransferFileStatus(std::string requestID, bool archive,
                               unsigned offset, unsigned limit, std::vector<FileTransferStatus*>& files);

    void listRequests(std::vector<JobStatus*>& jobs, std::vector<std::string>& inGivenStates,
                      std::string restrictToClientDN, std::string forDN, std::string VOname, std::string src, std::string dst);

    TransferJobs* getTransferJob(std::string jobId, bool archive);

    void getByJobIdReuse(std::vector<TransferJobs*>& jobs, std::map< std::string, std::list<TransferFiles> >& files);

    void getByJobId( std::vector< boost::tuple<std::string, std::string, std::string> >& distinct, std::map< std::string, std::list<TransferFiles> >& files);

    void getSe(Se* &se, std::string seName);

    unsigned int updateFileStatus(TransferFiles file, const std::string status);

    void addSe(std::string endpoint, std::string se_type, std::string site, std::string name, std::string state, std::string version, std::string host,
               std::string se_transfer_type, std::string se_transfer_protocol, std::string se_control_protocol, std::string gocdb_id);

    void updateSe(std::string endpoint, std::string se_type, std::string site, std::string name, std::string state, std::string version, std::string host,
                  std::string se_transfer_type, std::string se_transfer_protocol, std::string se_control_protocol, std::string gocdb_id);

    bool updateFileTransferStatus(double throughput, std::string job_id, int file_id, std::string transfer_status, std::string transfer_message,
                                  int process_id, double filesize, double duration, bool retry);

    bool updateJobTransferStatus(std::string job_id, const std::string status, int pid);

    void updateFileTransferProgressVector(std::vector<struct message_updater>& messages);

    void cancelJob(std::vector<std::string>& requestIDs);

    bool insertGrDPStorageCacheElement(std::string dlg_id, std::string dn, std::string cert_request, std::string priv_key, std::string voms_attrs);

    void updateGrDPStorageCacheElement(std::string dlg_id, std::string dn, std::string cert_request, std::string priv_key, std::string voms_attrs);

    CredCache* findGrDPStorageCacheElement(std::string delegationID, std::string dn);

    void deleteGrDPStorageCacheElement(std::string delegationID, std::string dn);

    void insertGrDPStorageElement(std::string dlg_id, std::string dn, std::string proxy, std::string voms_attrs, time_t termination_time);

    void updateGrDPStorageElement(std::string dlg_id, std::string dn, std::string proxy, std::string voms_attrs, time_t termination_time);

    Cred* findGrDPStorageElement(std::string delegationID, std::string dn);

    void deleteGrDPStorageElement(std::string delegationID, std::string dn);

    bool getDebugMode(std::string source_hostname, std::string destin_hostname);

    void setDebugMode(std::string source_hostname, std::string destin_hostname, std::string mode);

    void getSubmittedJobsReuse(std::vector<TransferJobs*>& jobs, const std::string & vos);

    void auditConfiguration(const std::string & dn, const std::string & config, const std::string & action);

    void fetchOptimizationConfig2(OptimizerSample* ops, const std::string & source_hostname, const std::string & destin_hostname);

    bool isCredentialExpired(const std::string & dlg_id, const std::string & dn);

    bool updateOptimizer();

    bool isTrAllowed(const std::string & source_se, const std::string & dest);

    int getSeOut(const std::string & source, const std::set<std::string> & destination);

    int getSeIn(const std::set<std::string> & source, const std::string & destination);

    void setAllowed(const std::string & job_id, int file_id, const std::string & source_se, const std::string & dest, int nostreams, int timeout, int buffersize);

    void setAllowedNoOptimize(const std::string & job_id, int file_id, const std::string & params);

    bool terminateReuseProcess(const std::string & jobId, int pid, const std::string & message);

    void forceFailTransfers(std::map<int, std::string>& collectJobs);

    void setPid(const std::string & jobId, int fileId, int pid);

    void setPidV(int pid, std::map<int,std::string>& pids);

    void revertToSubmitted();

    void backup(long* nJobs, long* nFiles);

    void forkFailedRevertState(const std::string & jobId, int fileId);

    void forkFailedRevertStateV(std::map<int,std::string>& pids);

    bool retryFromDead(std::vector<struct message_updater>& messages, bool diskFull);

    void blacklistSe(std::string se, std::string vo, std::string status, int timeout, std::string msg, std::string adm_dn);

    void blacklistDn(std::string dn, std::string msg, std::string adm_dn);

    void unblacklistSe(std::string se);

    void unblacklistDn(std::string dn);

    bool isSeBlacklisted(std::string se, std::string vo);

    bool allowSubmitForBlacklistedSe(std::string se);

    void allowSubmit(std::string ses, std::string vo, std::list<std::string>& notAllowed);

    boost::optional<int> getTimeoutForSe(std::string se);

    void getTimeoutForSe(std::string ses, std::map<std::string, int>& ret);

    bool isDnBlacklisted(std::string dn);

    bool isFileReadyState(int fileID);

    bool isFileReadyStateV(std::map<int,std::string>& fileIds);

    bool checkGroupExists(const std::string & groupName);

    void getGroupMembers(const std::string & groupName, std::vector<std::string>& groupMembers);

    void addMemberToGroup(const std::string & groupName, std::vector<std::string>& groupMembers);

    void deleteMembersFromGroup(const std::string & groupName, std::vector<std::string>& groupMembers);

    std::string getGroupForSe(const std::string se);

    void addLinkConfig(LinkConfig* cfg);
    void updateLinkConfig(LinkConfig* cfg);
    void deleteLinkConfig(std::string source, std::string destination);
    LinkConfig* getLinkConfig(std::string source, std::string destination);
    std::pair<std::string, std::string>* getSourceAndDestination(std::string symbolic_name);
    bool isGrInPair(std::string group);
    bool isShareOnly(std::string se);

    void addShareConfig(ShareConfig* cfg);
    void updateShareConfig(ShareConfig* cfg);
    void deleteShareConfig(std::string source, std::string destination, std::string vo);
    void deleteShareConfig(std::string source, std::string destination);
    ShareConfig* getShareConfig(std::string source, std::string destination, std::string vo);
    std::vector<ShareConfig*> getShareConfig(std::string source, std::string destination);

    virtual void addActivityConfig(std::string vo, std::string shares, bool active);
    virtual void updateActivityConfig(std::string vo, std::string shares, bool active);
    virtual void deleteActivityConfig(std::string vo);
    virtual bool isActivityConfigActive(std::string vo);
    virtual std::map< std::string, double > getActivityConfig(std::string vo);

    bool checkIfSeIsMemberOfAnotherGroup( const std::string & member);

    void addFileShareConfig(int file_id, std::string source, std::string destination, std::string vo);

    void getFilesForNewSeCfg(std::string source, std::string destination, std::string vo, std::vector<int>& out);

    void getFilesForNewGrCfg(std::string source, std::string destination, std::string vo, std::vector<int>& out);

    void delFileShareConfig(int file_id, std::string source, std::string destination, std::string vo);

    void delFileShareConfig(std::string groupd, std::string se);

    bool hasStandAloneSeCfgAssigned(int file_id, std::string vo);

    bool hasPairSeCfgAssigned(int file_id, std::string vo);

    bool hasPairGrCfgAssigned(int file_id, std::string vo);

    int countActiveTransfers(std::string source, std::string destination, std::string vo);

    int countActiveOutboundTransfersUsingDefaultCfg(std::string se, std::string vo);

    int countActiveInboundTransfersUsingDefaultCfg(std::string se, std::string vo);

    int sumUpVoShares (std::string source, std::string destination, std::set<std::string> vos);

    void setPriority(std::string jobId, int priority);

    void setSeProtocol(std::string protocol, std::string se, std::string state);

    void setRetry(int retry, const std::string & vo);

    int getRetry(const std::string & jobId);

    int getRetryTimes(const std::string & jobId, int fileId);

    void setRetryTransfer(const std::string & jobId, int fileId);

    void setMaxTimeInQueue(int afterXHours);

    void setToFailOldQueuedJobs(std::vector<std::string>& jobs);

    std::vector< std::pair<std::string, std::string> > getPairsForSe(std::string se);

    virtual std::vector<std::string> getAllActivityShareConf();

    std::vector<std::string> getAllStandAlloneCfgs();

    std::vector<std::string> getAllShareOnlyCfgs();

    int activeProcessesForThisHost();

    std::vector< std::pair<std::string, std::string> > getAllPairCfgs();

    void setFilesToNotUsed(std::string jobId, int fileIndex, std::vector<int>& files);

    std::vector< boost::tuple<std::string, std::string, int> >  getVOBringonlineMax();

    std::vector<struct message_bringonline> getBringOnlineFiles(std::string voName, std::string hostName, int maxValue);

    void bringOnlineReportStatus(const std::string & state, const std::string & message, const struct message_bringonline& msg);

    void addToken(const std::string & job_id, int file_id, const std::string & token);

    void getCredentials(std::string & vo_name, const std::string & job_id, int file_id, std::string & dn, std::string & dlg_id);

    void setMaxStageOp(const std::string& se, const std::string& vo, int val);

    double getSuccessRate(std::string source, std::string destination);

    double getAvgThroughput(std::string source, std::string destination);

    void updateProtocol(const std::string& jobId, int fileId, int nostreams, int timeout, int buffersize, double filesize);

    void cancelFilesInTheQueue(const std::string& se, const std::string& vo, std::set<std::string>& jobs);

    void cancelJobsInTheQueue(const std::string& dn, std::vector<std::string>& jobs);

    void transferLogFileVector(std::map<int, struct message_log>& messagesLog);

    std::vector<struct message_state> getStateOfTransfer(const std::string& jobId, int file_id);

    void getFilesForJob(const std::string& jobId, std::vector<int>& files);

    void getFilesForJobInCancelState(const std::string& jobId, std::vector<int>& files);

    void setFilesToWaiting(const std::string& se, const std::string& vo, int timeout);

    void setFilesToWaiting(const std::string& dn, int timeout);

    void cancelWaitingFiles(std::set<std::string>& jobs);

    void revertNotUsedFiles();

    void checkSanityState();

    void checkSchemaLoaded();

    void storeProfiling(const fts3::ProfilingSubsystem* prof);

    void setOptimizerMode(int mode);

    void setRetryTransfer(const std::string & jobId, int fileId, int retry, const std::string& reason);

    void getTransferRetries(int fileId, std::vector<FileRetry*>& retries);

    void updateHeartBeat(unsigned* index, unsigned* count, unsigned* start, unsigned* end, std::string service_name);

    unsigned int updateFileStatusReuse(TransferFiles file, const std::string status);

    void getCancelJob(std::vector<int>& requestIDs);

    void snapshot(const std::string & vo_name, const std::string & source_se, const std::string & dest_se, const std::string & endpoint, std::stringstream & result);

    bool getDrain();

    void setDrain(bool drain);

    void setBandwidthLimit(const std::string & source_hostname, const std::string & destination_hostname, int bandwidthLimit);

    std::string getBandwidthLimit();

    bool isProtocolUDT(const std::string & source_hostname, const std::string & destination_hostname);

    int getStreamsOptimization(const std::string & source_hostname, const std::string & destination_hostname);

    int getGlobalTimeout();

    void setGlobalTimeout(int timeout);

    int getSecPerMb();

    void setSecPerMb(int seconds);

    void setSourceMaxActive(const std::string & source_hostname, int maxActive);

    void setDestMaxActive(const std::string & destination_hostname, int maxActive);

    int getBufferOptimization();

    void getTransferJobStatusDetailed(std::string job_id, std::vector<boost::tuple<std::string, std::string, int, std::string, std::string> >& files);

    void getVOPairs(std::vector< boost::tuple<std::string, std::string, std::string> >& distinct);


    //NEW deletions and staging API
    //deletions						 //file_id / state / reason
    void updateDeletionsState(std::vector< boost::tuple<int, std::string, std::string, std::string> >& files);

    //file_id / surl / proxy
    void getFilesForDeletion(std::vector< boost::tuple<std::string, std::string, int, std::string, std::string> >& files);

    //job_id
    void cancelDeletion(std::vector<std::string>& files);

    //file_id / surl
    void getDeletionFilesForCanceling(std::vector< boost::tuple<int, std::string, std::string> >& files);

    void setMaxDeletionsPerEndpoint(int maxDeletions, const std::string & endpoint, const std::string & vo);
    int getMaxDeletionsPerEndpoint(const std::string & endpoint, const std::string & vo);



    //staging						//file_id / state / reason / token
    void updateStagingState(std::vector< boost::tuple<int, std::string, std::string, std::string> >& files);
    //file_id / surl / proxy / pinlifetime / bringonlineTimeout
    void getFilesForStaging(std::vector< boost::tuple<std::string, std::string, int, int, int, std::string, std::string, std::string> >& files);

    //job_id
    void cancelStaging(std::vector<std::string>& files);

    //file_id / surl / token
    void getStagingFilesForCanceling(std::vector< boost::tuple<int, std::string, std::string> >& files);

    void setMaxStagingPerEndpoint(int maxStaging, const std::string & endpoint, const std::string & vo);
    int getMaxStatingsPerEndpoint(const std::string & endpoint, const std::string & vo);
    
    void submitdelete(const std::string & jobId, const std::multimap<std::string,std::string>& rulsHost, 
    				const std::string & DN, const std::string & voName, const std::string & credID);    

};


void destroy_profiled_db(void *db);
