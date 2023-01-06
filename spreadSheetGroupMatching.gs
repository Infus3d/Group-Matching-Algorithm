function myFunction() {
  let inSheetId = "1Vjn_ytNpiXvkqM_X3xcGjuDfqBMHRIb5yT57pZXbKaI";
  let outSheetId = "1DY8LsWGU1lNiC6izHzzmIbGA38SBGyhRlxX2S43Jbig";

  let inSheet = SpreadsheetApp.openById(inSheetId).getSheetByName('Form Responses 1');
  let outSheet = SpreadsheetApp.openById(outSheetId).getSheets()[0];

  //[stRow, stCol] -> is the starting row & column of the preferences, not including the project names
  //n -> team/project count (they need to be equal)
  let stRow = 2, stCol = 5, n = inSheet.getLastRow()-stRow+1;

  let data = inSheet.getRange(1, 1, inSheet.getLastRow(), inSheet.getLastColumn()).getValues();  //entire data on the sheet
  let preferences = inSheet.getRange(stRow, stCol, inSheet.getLastRow()-stRow+1, inSheet.getLastColumn()-stCol+1).getValues(); //only the preferences

  Logger.log(data);
  Logger.log(preferences);
  
  var table = [], teams = [], projects = [];
  for(let i=stRow-1; i<data.length; i++)
    teams.push(data[i][1]);  //assuming team names(emails) are on the second column
  
  for(let i=stCol-1; i<data[0].length; i++)
    projects.push(data[0][i]);  //assuming project names are on the first row
  
  //getting the table of preferences as integer 'costs' 
  preferences.forEach(function(curRow){
    let r = [];
    for(let i=0; i<curRow.length; i++){
      if(curRow[i] == "") r.push(0);
      else r.push(Number(curRow[i][0]));
    }
    table.push(r);
  });

  Logger.log(teams);
  Logger.log(projects);
  Logger.log(table);

  //defining the sources needed for bitmask+dp
  let dp = [], track = [], ans = [];
  for(let i=0; i<n; i++){
    let r = [];
    for(let j=0; j<(1<<n); j++) 
      r.push(-1);
    dp.push(r.slice());
    track.push(r.slice());
    ans.push(0);
  }

  //Algorithm starts
  for(let i=0; i<n; i++)
    if(table[0][i] != 0){
      dp[0][(1<<i)] = table[0][i];
      track[0][(1<<i)] = i;
    }
  
  for(let i=1; i<n; i++){
    for(let mask=0; mask<(1<<n); mask++){
      for(let j=0; j<n; j++){
        if(dp[i-1][mask] != -1 && table[i][j] != 0 && (mask&(1<<j)) == 0){
          if(dp[i][mask|(1<<j)] == -1 || dp[i][mask|(1<<j)] > dp[i-1][mask] + table[i][j]){
            dp[i][mask|(1<<j)] = dp[i-1][mask] + table[i][j];
            track[i][mask|(1<<j)] = j;
          }
        }
      }
    }
  }

  //Tracking down the team assignments. dp[n-1][(1<<n)-1] contains the total minimal cost
  Logger.log(dp[n-1][(1<<n)-1]);
  let tempn = n-1, tempMask = (1<<n)-1;
  while(tempn >= 0){
    let cur = Number(track[tempn][tempMask]);
    ans[tempn] = cur;
    tempMask ^= (1<<cur);
    tempn--;
  }

  //outFile log
  let logOut = [["Total cost (min sum of preferences)", dp[n-1][(1<<n)-1], ""],
                ["", "", ""], 
                ["Team name(email)", "Assigned Project Name", "Preference #"]];
  
  for(let i=0; i<n; i++){
    let r = [teams[i], projects[ans[i]], table[i][ans[i]]];
    logOut.push(r);
  }
  Logger.log(logOut);

  outSheet.getRange(1, 1, logOut.length, 3).setValues(logOut);
  outSheet.autoResizeColumns(1, outSheet.getLastColumn());
}

