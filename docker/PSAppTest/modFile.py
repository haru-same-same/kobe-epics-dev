import os
import sys
from glob import glob

def main():
    work_dir, epics_ver, asyn_ver, user = sys.argv[1:5]
    
    # Path to the file which will be modified
    db_path = '/PSApp/PSClientApp/Db/'
    release_path = '/PSApp/configure/'
    src_path = '/PSApp/PSClientApp/src/'
    cmd_path = '/PSApp/iocBoot/iocPSClient/'
    
    # Dictionary of IP, port
    ip_dict = {PS1: '10.37.0.196:50001'}
    
    # Create list of .db and .proto file
    db_list = glob(db_path + '*.db')
    db_list = [os.path.basename(idb) for idb in db_list]
    proto_list = glob(db_path + '*.proto')
    proto_list = [os.path.basename(iproto) for iproto in ptroto_list]
    
    # Create RELEASE.local and edit
    cont_release = ['ASYN=' + work_dir + '/base-' + epics_ver + '/modules/asyn' + asyn_ver + '\n',
                    'STREAM=' + work_dir + '/base-' + epics_ver + '/modules/StreamDevice']
    with open(release_path + 'RELEASE.local', mode = 'a') as f_release:
        f_release.writelines(cont_release)
    
    # Edit src/Makefile
    cont_srcmk = ['PSClient_DBD += asyn.dbd\n',
                  'PSClient_DBD += stream.dbd\n',
                  'PSClient_DBD += drvAsynIPPort.dbd\n',
                  'PSClient_LIBS += asyn\n',
                  'PSClient_LIBS += stream']
    with open(src_path + 'Makefile', mode = 'a') as f_srcmk:
        f_srcmk.writelines(cont_srcmk)
    
    # Edit Db/Makefile
    cont_dbmk = ['DB += ' + idb + '\n' for idb in db_list]
    with open(db_path + 'Makefile', mode = 'a') as f_dbmk:
        f_dbmk.writelines(cont_dbmk)
    
    # Edit st.cmd
    cont_dbrcd = ['dbLoadRecords("db/' + idb + '")\n' for idb in db_list]
    cont_ipport = ['drvAsynIPPortConfigure("' + ikey + '", "' + ip_dict[ikey] + '")\n' for ikey in ip_dict.keys()]
    cont_envset = 'epicsEnvSet("STREAM_PROTOCOL_PATH", ".:../../PSClientApp/Db")\n'
    cont_cmd_i = cont_dbrcd.append(cont_envset)
    cont_cmd_i.extend(cont_ipport)
    cont_cmd_u = []
    cont_cmd_d = []
    find_flag = False
    with open(cmd_path + 'st.cmd', mode = 'r') as f_cmd:
        for row in f_cmd:
            if not find_flag:
                cont_cmd_u.append(row)
            else:
                cont_cmd_d.append(row)
            if '#dbLoadRecords("db/xxx.db","user=${USER}")' in row:
                find_flag = True
    
    cont_cmd = cont_cmd_u.extend(cont_cmd_i)
    cont_cmd.extend(cont_cmd_d)
    with open(cmd_path + 'st.cmd', mode = 'w') as f_cmd_w:
        f_cmd_w.writelines(cont_cmd)

if __name__ == '__main__':
    main()

