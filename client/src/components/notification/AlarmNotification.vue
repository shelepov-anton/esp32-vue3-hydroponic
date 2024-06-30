<template>
  <div />
</template>
<script lang="ts" setup>
import { useToast } from 'vue-toast-notification'
import { watch } from 'vue'
import { useConfigurationStore } from '@/store/configuration'
import { storeToRefs } from 'pinia'

const INFINITY = 9999999

const notification = useToast({
  dismissible: false,
  position: 'top',
  queue: false
})

const { isNotificationHidden, notificationMessages } = storeToRefs(useConfigurationStore())

const displayNotification = (message: string) => {
  notification.error(message, { duration: INFINITY })
}

watch(isNotificationHidden, (value: boolean) => {
  if (value) {
    notification.clear()
  }
})

watch(notificationMessages, (messages) => {
  console.log('notificationMessages', messages)
  notification.clear()
  for (const message of messages) {
    displayNotification(message)
  }
}, { deep: true })

</script>