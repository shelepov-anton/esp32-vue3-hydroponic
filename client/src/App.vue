<template>
  <main class="main-container">
    <AppHeader />
    <DayCounter :count="daysCount" :style="{ margin: '50px 0 20px 0' }" />
    <div>
      <Datalist :style="{ marginBottom: '30px' }" :ph-value="phValue" />
      <CustomButton @click="onStartClick" type="primary" :style="{marginBottom: '14px'}">
        {{ daysCount ? 'Редактировать' : 'Старт' }}
      </CustomButton>
      <CustomButton @click="onQuiteClick" type="secondary">
        Квитировать
      </CustomButton>
    </div>
    <ModalsContainer />
    <AlarmNotification />
  </main>
</template>

<script setup lang="ts">
import AppHeader from '@/components/header/AppHeader.vue'
import DayCounter from '@/components/DayCounter.vue'
import CustomButton from '@/components/ui/CustomButton.vue'
import Datalist from '@/components/Datalist.vue'
import { useModal, useModalSlot, VueFinalModal } from 'vue-final-modal'
import SettingsModal from '@/components/modal/SettingsModal.vue'
import { ModalsContainer } from 'vue-final-modal'
import { onMounted, ref, watch } from 'vue'
import { useConfigurationStore } from '@/store/configuration'
import { storeToRefs } from 'pinia'
import AlarmNotification from '@/components/notification/AlarmNotification.vue'
import socket from '@/api/websocket'
import { checkMessage, ControllerMessageType, formatMessage } from '@/utils/controller'

const settingsModal = useModal({
  component: VueFinalModal,
  slots: {
    default: useModalSlot({
      component: SettingsModal
    })
  }
})

// Days count

const { daysCount, isNotificationHidden } = storeToRefs(useConfigurationStore())

onMounted(() => {
  const storageDaysCount = Number(localStorage.getItem('daysCount'))
  if (storageDaysCount) {
    daysCount.value = storageDaysCount
  } else {
    settingsModal.open()
  }
})

watch(daysCount, () => {
  settingsModal.close()
})

// Controller data

const phValue = ref<number>(0)

socket.onmessage = (event) => {
  const message = event.data
  if (checkMessage(message, ControllerMessageType.PH_VALUE)) {
    phValue.value = Number(formatMessage(message))
  }
  if (checkMessage(message, ControllerMessageType.ALARM)) {
    useConfigurationStore().showNotification(formatMessage(message))
  }
}

// Methods

const onStartClick = () => {
  settingsModal.open()
}

const onQuiteClick = () => {
  useConfigurationStore().hideNotification()
}

</script>
<style lang="scss" scoped>
.main-container {
  display: flex;
  flex-direction: column;
  align-content: center;
  justify-content: space-between;
  padding: 2rem;
  height: 100%;
}
</style>
