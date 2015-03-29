rowMins_R <- function(X,cols = 1:ncol(X)){
  minRes <-c(length=length(cols))
  minInx <- min(cols)
  for(i in cols){ 
    minRes[i-minInx+1] <- min(X[,i])
    }
  return(minRes)
}